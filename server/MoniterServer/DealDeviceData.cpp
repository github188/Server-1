#include "DealDeviceData.h"
#include <cassert>
#include <sstream>
#include <fstream>
#include <algorithm>
#include "snmp.h"
#define OTL_ORA12C_R2 // Compile OTL 4.0/OCI12
#define OTL_STL // Enable STL compatibility mode
#define OTL_ORA_UTF8
#define OTL_STREAM_POOLING_ON
#define OTL_ORA_TIMESTAMP
#include "otlv4_h2/otlv4.h"
#include "json/json.h"
#include "RAII.h"
#include "LogMacros.h"
#include "ParseIni.h"

static SNMPAPI_STATUS CALLBACK snmpCallBack(HSNMP_SESSION hSession, HWND, UINT, WPARAM, LPARAM, LPVOID lpClientData)
{
	/*************************************************
	cpu1,cpu2,cpu3.........mutiple cpu, then get average. or single cpu directly get
	memoryTotal,memoryUsed..............then memoryUsed/memoryTotal. or single memory directly get
	diskTotal1,diskUsed1,diskTotal2,diskUsed2.....then (diskUsed1+diskUsed2+.....)/(diskTotal1+diskTotal2....) or no disk
	netIn1,netOut1,netIn2,netOut2..........then netIn1+netOut1+netIn2+netOut2..........= all_last
	netIn1,netOut1,netIn2,netOut2..........then netIn1+netOut1+netIn2+netOut2..........= all_this
	(all_this-all_last) / interval=speed ,then speed /ifspeed ???
	*************************************************/

	/*std::chrono::time_point<std::chrono::system_clock> start, end;
	start = std::chrono::system_clock::now();*/

	auto impl = static_cast<Platform::Impl*>(lpClientData);
	bool all_get = false;
	Platform::DeviceCompareAttribute dca;

	auto getDeviceData = [&impl, &all_get, &hSession, &dca]()->int
	{
		auto mutex_ptr = impl->mutex_ptr;
		auto snmp_ptr = impl->this_ptr->getSnmp();
		const auto& device_compare_attribute = impl->this_ptr->getDeviceCompareAttribute();
		const auto& model_oid = impl->this_ptr->getModelOid();
		auto& device_data = impl->this_ptr->getDeviceData();
#if _MSC_VER >=1800
		std::lock_guard<std::mutex> lock(*mutex_ptr);
#else
		boost::lock_guard<boost::mutex> lock(*mutex_ptr);
#endif
		auto& is_oid_whole = impl->this_ptr->getIsOidWhole();
		try
		{
			void* recvPdu = nullptr;
			void* SrcEntity = nullptr;
			void * DstEntity = nullptr;
			void* context = nullptr;
			snmp_ptr->recvMessage(hSession, SrcEntity, DstEntity, context, recvPdu);
			WwFoundation::RAII recvMessage_raii([snmp_ptr, recvPdu, SrcEntity, DstEntity, context]()
			{
				snmp_ptr->releasePdu(recvPdu);
				snmp_ptr->releaseEntity(SrcEntity);
				snmp_ptr->releaseEntity(DstEntity);
				snmp_ptr->releaseContext(context);
			});

			std::string ip_temp;
			snmp_ptr->translateEntityToIp(SrcEntity, ip_temp);
			for (auto iter = device_compare_attribute.begin(); iter != device_compare_attribute.end(); ++iter)
			{
				if (iter->deviceIp == ip_temp)
				{
					dca = *iter;
					break;
				}
			}

			void* vblist = nullptr;
			snmp_ptr->getPduData(recvPdu, vblist);
			WwFoundation::RAII getPduData_raii([snmp_ptr, vblist]()
			{
				snmp_ptr->releaseVbl(vblist);
			});

			if (is_oid_whole[dca])
			{
				int vbl_count = 0;
				snmp_ptr->getCountVbl(vblist, vbl_count);
				auto iter = model_oid.find(dca.deviceModel);
				auto oid_group = iter->second;
				for (int i = 1; i <= vbl_count; ++i)
				{
					smiOID name;
					smiVALUE value;
					snmp_ptr->getVb(vblist, &name, i, &value);
					std::string oid_str;
					snmp_ptr->translateOidToStr(&name, oid_str);
					std::string key_name;
					for (auto iter1 = oid_group.begin(); iter1 != oid_group.end(); ++iter1)
					{
						if (iter1->second == oid_str)
						{
							key_name = iter1->first;
							oid_group.erase(key_name); //reduce times of the circle
							break;
						}
					}
					switch (value.syntax)
					{
					case SNMP_SYNTAX_INT32:
					{
						device_data[dca].emplace(std::make_pair(key_name, static_cast<uint64_t>(value.value.sNumber)));
						break;
					}
					case SNMP_SYNTAX_UINT32:
					case SNMP_SYNTAX_CNTR32:
					case SNMP_SYNTAX_GAUGE32:
					case SNMP_SYNTAX_TIMETICKS:
					{
						device_data[dca].emplace(std::make_pair(key_name, static_cast<uint64_t>(value.value.uNumber)));
						break;
					}
					case SNMP_SYNTAX_CNTR64:
					{
						uint64_t number = (static_cast<uint64_t>(value.value.hNumber.hipart)) << 32 | value.value.hNumber.lopart;
						device_data[dca].emplace(std::make_pair(key_name, number));
						break;
					}
					case SNMP_SYNTAX_OCTETS:
					case SNMP_SYNTAX_BITS:
					case SNMP_SYNTAX_OPAQUE:
					case SNMP_SYNTAX_IPADDR:
					case SNMP_SYNTAX_NSAPADDR:
					{
						char desc[500] = { 0 };
						memcpy(desc, value.value.string.ptr, value.value.string.len);
						std::string str = desc;
						//std::string str1 = reinterpret_cast<char*>(value.value.string.ptr);
						device_data[dca].emplace(std::make_pair(key_name, str));
						SnmpFreeDescriptor(value.syntax, &(value.value.string));
						break;
					}
					case SNMP_SYNTAX_OID:
					{
						//SnmpOidToStr(&value.value.oid, 1000, s);
						//std::cout << s << std::endl;
						//SnmpFreeDescriptor(value.syntax, reinterpret_cast<smiLPOPAQUE>(&(value.value.oid)));
						break;
					}
					case SNMP_SYNTAX_NULL:
					case SNMP_SYNTAX_NOSUCHOBJECT:
					case SNMP_SYNTAX_NOSUCHINSTANCE:
					case SNMP_SYNTAX_ENDOFMIBVIEW:
					{
						std::cout << "oid is invalid" << std::endl;
						break;
					}
					default:
					{
						std::cout << "out of win define range" << std::endl;
						break;
					}
					}
				}

				auto& oid_group1 = iter->second;
				if (device_data[dca].size() == oid_group1.size())
				{
					all_get = true;
					is_oid_whole[dca] = false;
				}
			}
		}
		catch (const std::exception& e)
		{
			auto& device_port_flow = impl->this_ptr->getPortflow();
			MONITERSERVER_ERROR("%s, get device data failed, device id is %s ", e.what(), dca.deviceId.c_str());
			device_data[dca].clear();
			device_port_flow[dca].clear();
			is_oid_whole[dca] = false;
			return -1;
		}
		return 0;
	};
	auto writeToDB = [&impl, &dca]()
	{
		auto db_ptr = impl->db;
		auto &device_port_flow = impl->this_ptr->getPortflow();
		auto &device_data = impl->this_ptr->getDeviceData();
#if _MSC_VER >=1800
		auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
#else
		auto t = boost::chrono::system_clock::to_time_t(boost::chrono::system_clock::now());
#endif
		std::ostringstream time;
		time << (std::localtime(&t)->tm_year + 1900) << "-" << (std::localtime(&t)->tm_mon + 1) << "-" << std::localtime(&t)->tm_mday
			<< " " << std::localtime(&t)->tm_hour << ":" << std::localtime(&t)->tm_min << ":" << std::localtime(&t)->tm_sec;
		std::ostringstream sql;
		std::ostringstream sql_value;
		//insert into devicedata(u_number,upload_time) values('22',to_date('2017-7-13 11:50:12','YYYY-MM-DD HH24:MI:SS'))
		sql << "insert into " << impl->this_ptr->cm_.deviceDataTable_name << "(" << Platform::DEVICE_ID
			<< "," << Platform::DEVICE_UPLOAD_TIME;
		sql_value << "values (" << "'" << dca.deviceId << "'" << "," << "to_date('" << time.str() << "','YYYY-MM-DD HH24:MI:SS')";

		auto iter_sys_desc = device_data[dca].find("sysDesc");
		if (iter_sys_desc != device_data[dca].end()) // has system_desc
		{
			sql << "," << Platform::DEVICE_SYSTEM_DESCRIPTION;
#if _MSC_VER >=1800
			sql_value << "," << "'" << (std::any_cast<std::string>(iter_sys_desc->second)) << "'";
#else
			sql_value << "," << "'" << (boost::any_cast<std::string>(iter_sys_desc->second)) << "'";
#endif	
			device_data[dca].erase(iter_sys_desc); //remove this element
		}

		auto iter_cpu = device_data[dca].find("cpu");
		if (iter_cpu != device_data[dca].end())//just has single cpu
		{
			sql << "," << Platform::DEVICE_CPU_RATE;
#if _MSC_VER >=1800
			sql_value << "," << "'" << std::any_cast<uint64_t>(iter_cpu->second) << "'";
#else
			sql_value << "," << "'" << boost::any_cast<uint64_t>(iter_cpu->second) << "'";
#endif	
			device_data[dca].erase(iter_cpu); //remove this element
		}
		else // has several cpus or no cpu
		{
			bool exit_loop = false;
			auto iter_cpu1 = device_data[dca].find("cpu1");
			if (iter_cpu1 != device_data[dca].end()) // has several cpus
			{
				uint64_t cpu_rate_all = 0;
				int cpu_count = 0;
				for (auto iter = device_data[dca].begin(); iter != device_data[dca].end();)
				{
					auto pos = (iter->first).find("cpu");
					if (pos != std::string::npos) //find one like cpux(x=1 or 2 or 3.......)
					{
						exit_loop = true; //since sotr , once goto else branch,then no cpux exit
#if _MSC_VER >=1800
						cpu_rate_all = cpu_rate_all + std::any_cast<uint64_t>(iter->second);
#else
						cpu_rate_all = cpu_rate_all + boost::any_cast<uint64_t>(iter->second);
#endif					
						++cpu_count;
						iter = device_data[dca].erase(iter); //remove this element
					}
					else
					{
						if (exit_loop)
							break;
						++iter;
					}
				}
				auto cpu_use_rate = cpu_rate_all *(float)1.0 / cpu_count;
				sql << "," << Platform::DEVICE_CPU_RATE;
				sql_value << "," << "'" << cpu_use_rate << "'";
			}
		}

		auto iter_diskTotal = device_data[dca].find("diskTotal");
		if (iter_diskTotal != device_data[dca].end()) //just has single disk
		{
#if _MSC_VER >=1800
			auto diskTotal = std::any_cast<uint64_t>(iter_diskTotal->second);
#else
			auto diskTotal = boost::any_cast<uint64_t>(iter_diskTotal->second);
#endif
			device_data[dca].erase(iter_diskTotal); //remove this element

			auto iter_diskUsed = device_data[dca].find("diskUsed");
			if (iter_diskUsed != device_data[dca].end())
			{
#if _MSC_VER >=1800
				auto diskUsed = std::any_cast<uint64_t>(iter_diskUsed->second);
#else
				auto diskUsed = boost::any_cast<uint64_t>(iter_diskUsed->second);
#endif
				auto disk_use_rate = diskUsed * 100 / diskTotal;
				sql << "," << Platform::DEVICE_DISK_RATE;
				sql_value << "," << "'" << disk_use_rate << "'";
				device_data[dca].erase(iter_diskUsed); //remove this element
			}
		}
		else //has several disks or no disk
		{
			std::vector<uint64_t> diskTotal;
			std::vector<uint64_t> diskUsed;
			uint64_t diskTotal_all = 0;
			uint64_t diskUsed_all = 0;
			bool exit_loop = false;

			auto iter_diskTotal1 = device_data[dca].find("diskTotal1");
			if (iter_diskTotal1 != device_data[dca].end()) // has several disks
			{
				for (auto iter = device_data[dca].begin(); iter != device_data[dca].end();)
				{
					auto pos = (iter->first).find("diskTotal");
					auto pos1 = (iter->first).find("diskUsed");
					if (pos != std::string::npos || pos1 != std::string::npos)
					{
						exit_loop = true; //since sort , once goto else branch,then no diskTotalx or diskUsedx exit
						if (pos != std::string::npos)//find one like diskTotalx(x=1 or 2 or 3.......)
						{
#if _MSC_VER >=1800
							diskTotal.emplace_back(std::any_cast<uint64_t>(iter->second));
							diskTotal_all = diskTotal_all + std::any_cast<uint64_t>(iter->second);
#else
							diskTotal.emplace_back(boost::any_cast<uint64_t>(iter->second));
							diskTotal_all = diskTotal_all + boost::any_cast<uint64_t>(iter->second);
#endif
							iter = device_data[dca].erase(iter);
						}
						else //find one like diskUsedx(x=1 or 2 or 3.......)
						{
#if _MSC_VER >=1800
							diskUsed.emplace_back(std::any_cast<uint64_t>(iter->second));
							diskUsed_all = diskUsed_all + std::any_cast<uint64_t>(iter->second);
#else
							diskUsed.emplace_back(boost::any_cast<uint64_t>(iter->second));
							diskUsed_all = diskUsed_all + boost::any_cast<uint64_t>(iter->second);
#endif					
							iter = device_data[dca].erase(iter);
						}
					}
					else
					{
						if (exit_loop)
							break;
						++iter;
					}
				}
				auto disk_use_rate = diskUsed_all * (float)100.0 / diskTotal_all;
				sql << "," << Platform::DEVICE_DISK_RATE;
				sql_value << "," << "'" << disk_use_rate << "'";
			}
		}

		auto iter_memory = device_data[dca].find("memory");
		if (iter_memory != device_data[dca].end()) //do not need calculate,direct get
		{
#if _MSC_VER >=1800
			auto mem_use_rate = std::any_cast<uint64_t>(iter_memory->second);
#else
			auto mem_use_rate = boost::any_cast<uint64_t>(iter_memory->second);
#endif		
			device_data[dca].erase(iter_memory); //remove this element
			sql << "," << Platform::DEVICE_MEMORY_RATE;
			sql_value << "," << "'" << mem_use_rate << "'";
		}
		else //need calculate
		{
			auto iter_memoryTotal = device_data[dca].find("memoryTotal");
			if (iter_memoryTotal != device_data[dca].end()) //has memory
			{
#if _MSC_VER >=1800
				auto memoryTotal = std::any_cast<uint64_t>(iter_memoryTotal->second);
#else
				auto memoryTotal = boost::any_cast<uint64_t>(iter_memoryTotal->second);
#endif
				device_data[dca].erase(iter_memoryTotal); //remove this element
				auto iter_memoryUsed = device_data[dca].find("memoryUsed");
				if (iter_memoryUsed != device_data[dca].end())
				{
#if _MSC_VER >=1800
					auto memoryUsed = std::any_cast<uint64_t>(iter_memoryUsed->second);
#else
					auto memoryUsed = boost::any_cast<uint64_t>(iter_memoryUsed->second);
#endif
					auto mem_use_rate = memoryUsed * 100 / memoryTotal;
					sql << "," << Platform::DEVICE_MEMORY_RATE;
					sql_value << "," << "'" << mem_use_rate << "'";
					device_data[dca].erase(iter_memoryUsed); //remove this element
				}
			}
		}

		auto iter_temperature = device_data[dca].find("temperature");
		if (iter_temperature != device_data[dca].end())// has temperature
		{
			sql << "," << Platform::DEVICE_CPU_TEMPERATURE;
#if _MSC_VER >=1800
			sql_value << "," << "'" << (std::any_cast<uint64_t>(iter_temperature->second)) << "'";
#else
			sql_value << "," << "'" << (boost::any_cast<uint64_t>(iter_temperature->second)) << "'";
#endif
			device_data[dca].erase(iter_temperature); //remove this element
		}

		// at last, just leave net
		int port_count = device_data[dca].size() / 3;
		if (port_count > 0)
		{
			std::vector<Platform::PortFlow> this_port_flow;
			Platform::PortFlow port_flow;
			auto iter_net = device_data[dca].begin();
			for (int i = 0; i < port_count; ++i, ++iter_net)
			{
				auto iter_net_temp = iter_net;
#if _MSC_VER >=1800
				port_flow.ifSpeed = static_cast<int32_t>(std::any_cast<uint64_t>(iter_net_temp->second));
#else
				port_flow.ifSpeed = static_cast<int32_t>(boost::any_cast<uint64_t>(iter_net_temp->second));
#endif		
				for (int j = 0; j < port_count; ++j)
					++iter_net_temp;
#if _MSC_VER >=1800
				port_flow.in = static_cast<int32_t>(std::any_cast<uint64_t>(iter_net_temp->second));
#else
				port_flow.in = static_cast<int32_t>(boost::any_cast<uint64_t>(iter_net_temp->second));
#endif
				for (int p = 0; p < port_count; ++p)
					++iter_net_temp;
#if _MSC_VER >=1800
				port_flow.out = static_cast<int32_t>(std::any_cast<uint64_t>(iter_net_temp->second));
#else
				port_flow.out = static_cast<int32_t>(boost::any_cast<uint64_t>(iter_net_temp->second));
#endif		
				this_port_flow.emplace_back(port_flow);
			}
			auto iter = device_port_flow.find(dca);
			if (iter != device_port_flow.end() && !(iter->second.empty())) //this device has last port_flow data
			{
				std::vector<float> net_use_rate;
				float net_useRate_all = 0;
				auto last_port_flow = iter->second;
				assert(this_port_flow.size() == last_port_flow.size());
				for (int i = 0; i < static_cast<int>(last_port_flow.size()); i++)
				{
					auto change_flow_in = this_port_flow[i].in - last_port_flow[i].in;
					auto change_flow_out = this_port_flow[i].out - last_port_flow[i].out;
					auto change_flow = change_flow_in + change_flow_out; //byte

					auto speed = (float)1.0* change_flow * 8 / (impl->this_ptr->cm_.moniter_rate / (float)1000.0);
					auto use_rate = speed * 100 / this_port_flow[i].ifSpeed;
					net_useRate_all = net_useRate_all + use_rate;
					net_use_rate.emplace_back(use_rate);
				}
				float net_useRate_average = net_useRate_all / (net_use_rate.size());
				if (net_useRate_average > (float)100)
				{
					MONITERSERVER_INFO("the device(id:%s) maybe reboot,net_useRate_average is %f", dca.deviceId.c_str(), net_useRate_average);
					net_useRate_average = 0;
				}
				//if (this_port_flow.size() > 1)
				//{
				auto iter_max = std::max_element(net_use_rate.begin(), net_use_rate.end());
				sql << "," << Platform::DEVICE_PORT_BANDWIDTH_MAX;
				if (*iter_max < (float)100)
					sql_value << "," << "'" << *iter_max << "'";
				else
					sql_value << "," << "'" << 0 << "'"; //reboot,so 0	
				//}

				sql << "," << Platform::DEVICE_BANDWIDTH_USE_AVERAGE;
				sql_value << "," << "'" << net_useRate_average << "'";
				iter->second.swap(this_port_flow);//this_port_flow will become the last_port_flow for the next request
			}
			else // no last port_flow data,this time do not write db
				device_port_flow[dca] = this_port_flow;
		}
		device_data[dca].clear();

		sql << ")";
		sql_value << ")";
		sql << sql_value.str();
		//end = std::chrono::system_clock::now();
		//int elapsed_seconds = std::chrono::duration_cast<std::chrono::milliseconds>
		//	(end - start).count();
		bool need_rlogon = false;
	Retry:
		try
		{
			if (need_rlogon)
			{
				std::ostringstream message;
				message << impl->this_ptr->cm_.dbUser_name << "/" << impl->this_ptr->cm_.db_password << "@" <<
					impl->this_ptr->cm_.db_ip << ":" << impl->this_ptr->cm_.db_port << "/" << impl->this_ptr->cm_.db_name;
				db_ptr->rlogon(message.str().c_str());
			}

			otl_nocommit_stream o1(1, sql.str().c_str(), *db_ptr);
			db_ptr->commit();
			o1.close(true);
			need_rlogon = false;
		}
		catch (otl_exception& p)// intercept OTL exceptions. ex:ORA-12543: TNS:destination host unreachable
		{
			MONITERSERVER_ERROR("%s", p.msg);// error message
			MONITERSERVER_ERROR("%s", p.stm_text);// SQL that caused the error
			MONITERSERVER_ERROR("%s", p.var_info);//the variable that caused the error
			if (db_ptr->connected == 1)
				db_ptr->logoff();
#if _MSC_VER >=1800
			std::this_thread::sleep_for(std::chrono::milliseconds(3000));
#else
			boost::this_thread::sleep_for(boost::chrono::milliseconds(3000));
#endif
			need_rlogon = true;
			goto Retry;
		}
	};

	auto ret = getDeviceData();
	if (ret != 0) // get device date failed
		return (SNMPAPI_STATUS)-1;
	if (all_get)
	{
		all_get = false;
		writeToDB();
	}
	/*auto end1 = std::chrono::system_clock::now();
		int elapsed_seconds1 = std::chrono::duration_cast<std::chrono::milliseconds>
			(end1 - start).count();
		printf("%d\n", elapsed_seconds1);*/
	return 0;
}

Platform::DealDeviceData::DealDeviceData() :run_(true), device_count_(0),
snmp_(std::make_shared<Snmp>(Snmp())), moniterThread_exception_exit_(false) {}

void Platform::DealDeviceData::init()
{
	getMessageFromIniConfigure(INI_CONFIGURE_DIR);
	load_.resize(cm_.load_count);
	snmp_->init();
	makeLoadsession();
#if _MSC_VER >=1800
	thread_moniter_ = std::thread(std::bind(&DealDeviceData::deviceMonitorThread, this));
#else
	thread_moniter_ = boost::thread(std::bind(&DealDeviceData::deviceMonitorThread, this));
#endif
}

void Platform::DealDeviceData::freeAllResource()
{
	run_ = false;
	if (thread_moniter_.joinable())
		thread_moniter_.join();
	for (auto iter = load_.begin(); iter != load_.end(); ++iter)
	{
		for (auto iter_v = (*iter)->begin(); iter_v != (*iter)->end(); ++iter_v)
		{
			releaseDeviceResource(nullptr, iter_v->context, iter_v->vbl, iter_v->manager_entity, iter_v->agent_entity, iter_v->pdu);
		}
		snmp_->releaseSession(load_session_[*iter]);
	}
}

void Platform::DealDeviceData::getMessageFromIniConfigure(const std::string dir)
{
	WwFoundation::ParseIni parser(dir);
	parser.parse();
	cm_.dbUser_name = parser.getValue("DbConfig", "DbUserName");
	cm_.db_ip = parser.getValue("DbConfig", "DbIp");
	cm_.db_name = parser.getValue("DbConfig", "DbName");
	cm_.db_password = parser.getValue("DbConfig", "DbPassword");
	cm_.db_port = parser.getValue("DbConfig", "DbPort");
	cm_.deviceDataTable_name = parser.getValue("DbConfig", "DeviceDataTableName");
	cm_.deviceTable_name = parser.getValue("DbConfig", "DeviceTableName");
	cm_.load_count = std::atoi(parser.getValue("ComConfig", "LoadCount").c_str());
	cm_.moniter_rate = std::atoi(parser.getValue("ComConfig", "MoniterRate").c_str());
}

void Platform::DealDeviceData::getMessageFromConfigure(const std::string dir)
{
	std::ifstream ifs;
	ifs.open(dir);
	if (!ifs.is_open() && device_compare_attribute_.empty()) //keep on running when the configure be lost, if the process is running
		throw std::logic_error("open json file failed, maybe the file is not existent");
	WwFoundation::RAII raii([&ifs]() {ifs.close(); });

	Json::Reader reader;
	Json::Value root;
	if (!reader.parse(ifs, root, false) && device_compare_attribute_.empty())//keep on running when parse the configure unsuccessfully  if the process is running
		throw std::runtime_error("parse json failed, maybe the format is wrong");

	auto device_model_name = root.getMemberNames();
	auto device_model_count = device_model_name.size();
	for (auto index = 0; index < static_cast<int>(device_model_count); ++index)
	{
		auto device_model_name_each = device_model_name[index];
		auto device_oid = root[device_model_name_each];
		auto oid_name = device_oid.getMemberNames();
		auto oid_name_count = oid_name.size();

		std::unordered_map<std::string, std::string> name_oid;
		for (int i = 0; i < static_cast<int>(oid_name_count); ++i)
		{
			auto oid_name_each = oid_name[i];
			auto oid = device_oid[oid_name_each].asString();
			name_oid.emplace(std::make_pair(oid_name_each, oid));
		}
		model_oid_.emplace(std::make_pair(device_model_name_each, name_oid));
	}
}

void Platform::DealDeviceData::getDeviceIpFromDB(std::vector<DeviceCompareAttribute>& device_compare_attribute, otl_connect & db)
{
	std::vector<std::string> device_model_name;
	for (auto iter = model_oid_.begin(); iter != model_oid_.end(); ++iter)
	{
		auto device_model_name_temp = iter->first;
		device_model_name.emplace_back(device_model_name_temp);
	}
	std::ostringstream message;
	message << "select " << DEVICE_MODEL << "," << DEVICE_IP << "," << DEVICE_ID << " from "
		<< cm_.deviceTable_name << " where " << DEVICE_MODEL << " in('";
	auto size = device_model_name.size();
	for (auto i = 0; i < static_cast<int>(size - 1); ++i)
		message << device_model_name.at(i) << "','";
	message << device_model_name.at(size - 1) << "')";
	auto sql = message.str();

	otl_stream o(size, sql.c_str(), db);//otl_stream just for select, else use otl_nocommit_stream, since select do not need to commit
	std::string DEVICEMODEL, DEVICEID, DEVICEIP = "";
	while (!o.eof())
	{
		o >> DEVICEMODEL >> DEVICEIP >> DEVICEID;
		DeviceCompareAttribute dcb;
		dcb.deviceModel = DEVICEMODEL;
		dcb.deviceIp = DEVICEIP;
		dcb.deviceId = DEVICEID;
		device_compare_attribute.emplace_back(dcb);
	}
	o.close(true); //reuse this stream 
}

bool Platform::DealDeviceData::isDeviceChange(std::vector<DeviceCompareAttribute>& device_compare_attribute, std::vector<DeviceCompareAttribute>& add_device, std::vector<DeviceCompareAttribute>& decrease_device)
{
	if (device_compare_attribute_ == device_compare_attribute)
		return false;
	else
	{
		if (!device_compare_attribute_.empty())
		{
			for (auto iter = device_compare_attribute.begin(); iter != device_compare_attribute.end(); ++iter)
			{
				auto iter_ = std::find(device_compare_attribute_.begin(), device_compare_attribute_.end(), *iter);
				if (iter_ != device_compare_attribute_.end()) //both has this element
					device_compare_attribute_.erase(iter_);
				else //has added device
					add_device.emplace_back(*iter);
			}
			if (!device_compare_attribute_.empty())//has decreasing device
				decrease_device.swap(device_compare_attribute_);
			device_compare_attribute_.swap(device_compare_attribute);
		}
		else
		{
			add_device = device_compare_attribute;
			device_compare_attribute_.swap(device_compare_attribute);
		}
		return true;
	}
}

void Platform::DealDeviceData::setDevcieAndLoadBalancing(std::vector<DeviceCompareAttribute>& add, std::vector<DeviceCompareAttribute>& decrease)
{
	if (!add.empty())
	{
		std::sort(load_.begin(), load_.end(), [](const std::shared_ptr<std::vector<DeviceAttribute>>& p1,
			const std::shared_ptr<std::vector<DeviceAttribute>>& p2)
		{return p1->size() < p2->size(); }); //first  up sort 

		device_count_ = device_count_ + add.size() - decrease.size();
		int average = device_count_ / cm_.load_count;
		int remain = add.size();// need deal count

		for (auto iter_s = load_.begin(); iter_s != load_.end(); ++iter_s)
		{
			auto size = (*iter_s)->size();
			auto iter = add.begin();
			auto deal = [&iter, &iter_s, this](int count)
			{
				for (int i = 0; i < count; ++i)
				{
					DeviceAttribute da;
					memset(&da, 0, sizeof(DeviceAttribute));
					da.dca = *iter;
					da.session = load_session_[*iter_s];
					auto ret = setSnmpParameter(da.session, da.manager_entity, da.context, da.vbl,
						da.agent_entity, da.pdu, *iter);
					if (ret == 0)//setSnmpParameter ok
					{
						(*iter_s)->emplace_back(da);
						device_port_flow_[*iter];//make empty value,make sure the key exists when using
						device_data_[*iter];//make empty value
					}
					else
					{
						//	LOG_WARN("add device failed,model:%s, ip:%s, id:%s", da.dca.deviceModel, da.dca.deviceIp, da.dca.deviceId);
						auto iter_dca = std::find(device_compare_attribute_.begin(), device_compare_attribute_.end(), da.dca);
						device_compare_attribute_.erase(iter_dca);
					}
					++iter;
				}
			};
			if (static_cast<int>(size) < average)
			{
				auto lack = average - static_cast<int>(size);
				if (remain > lack) // more enough to full
				{
					deal(lack);
					add.erase(add.begin(), add.begin() + lack); //remove the used element from add
					remain = remain - lack;//remove used
				}
				else // no enough to full or just enough
				{
					deal(remain);
					remain = remain - lack;//remove used
					break;// nothing leave,do not need circle any more
				}
			}
			else
				break; //1.since after sort,the device count of next load can not be smaller than average until end 
					//2. average equal 0 ,because of all device count less than load count ,ex: 3 device ,5 load, 3/5=0
		}
		if (remain > 0) //all full to average in above circle,still leave someting ,then the leaving count must be less than the load
		{
			auto iter_s = load_.begin();
			for (auto iter = add.begin(); iter != add.end(); ++iter)
			{
				DeviceAttribute da;
				memset(&da, 0, sizeof(DeviceAttribute));
				da.dca = *iter;
				da.session = load_session_[*iter_s];
				auto ret = setSnmpParameter(da.session, da.manager_entity, da.context, da.vbl,
					da.agent_entity, da.pdu, *iter);
				if (ret == 0)//setSnmpParameter ok
				{
					(*iter_s)->emplace_back(da);
					++iter_s;
					device_port_flow_[da.dca];//make empty value,make sure the key exists when using
					device_data_[da.dca];//make empty value
				}
				else
				{
					//LOG_WARN("add device failed,model:%s, ip:%s, id:%s", da.dca.deviceModel, da.dca.deviceIp, da.dca.deviceId);
					auto iter_dca = std::find(device_compare_attribute_.begin(), device_compare_attribute_.end(), da.dca);
					device_compare_attribute_.erase(iter_dca);
				}
			}
		}
	}
	if (!decrease.empty())
	{
		bool is_find = false;
		for (auto iter_decrease = decrease.begin(); iter_decrease != decrease.end(); ++iter_decrease)
		{
			for (auto iter_s = load_.begin(); iter_s != load_.end(); ++iter_s)
			{
				for (auto iter = (*iter_s)->begin(); iter != (*iter_s)->end(); ++iter)
				{
					if (*iter_decrease == (*iter).dca)
					{
						is_find = true;
						releaseDeviceResource(nullptr, (*iter).context, (*iter).vbl, //release resource
							(*iter).manager_entity, (*iter).agent_entity, (*iter).pdu);
						device_port_flow_.erase((*iter).dca);
						device_data_.erase((*iter).dca);
						is_oid_whole_.erase(iter->dca);
						(*iter_s)->erase(iter);//delete the decrease device
						break;
					}
				}
				if (is_find)
				{
					is_find = false;
					break;
				}
			}
		}
	}
}

void Platform::DealDeviceData::makeLoadsession()
{
	auto shared_this = shared_from_this();
	std::ostringstream message;
	message << cm_.dbUser_name << "/" << cm_.db_password << "@" <<
		cm_.db_ip << ":" << cm_.db_port << "/" << cm_.db_name;
	for (int i = 0; i < cm_.load_count; ++i)
	{
		auto impl = std::make_shared<Impl>(Impl());
		impl->db = std::shared_ptr<otl_connect>(new otl_connect);
		impl->this_ptr = shared_this;
#if _MSC_VER >=1800
		auto mutex_ptr = std::shared_ptr<std::mutex>(new std::mutex);
#else
		auto mutex_ptr = std::shared_ptr<boost::mutex>(new boost::mutex);
#endif
		impl->mutex_ptr = mutex_ptr;
		void* session;
		snmp_->makeSession(snmpCallBack, impl.get(), session);

		auto ptr = std::make_shared<std::vector<DeviceAttribute>>(std::vector<DeviceAttribute>());
		load_[i] = ptr;
		load_session_.emplace(std::make_pair(ptr, session));
		load_mutex_.emplace(std::make_pair(ptr, mutex_ptr));
		impl_.emplace_back(impl);

		//connect to db for each load_session
		otl_connect::otl_initialize(true); // initialize OCI environment,mutiple thread enable,but not safety

	Retry:
		try
		{
			impl->db->rlogon(message.str().c_str());
		}
		catch (otl_exception& p)// intercept OTL exceptions. ex:ORA-12543: TNS:destination host unreachable
		{
			MONITERSERVER_ERROR("%s", p.msg);//error message
			MONITERSERVER_ERROR("%s", p.stm_text);//SQL that caused the error
			MONITERSERVER_ERROR("%s", p.var_info);//the variable that caused the error

			if (impl->db->connected == 1)
				impl->db->logoff();
#if _MSC_VER >=1800
			std::this_thread::sleep_for(std::chrono::milliseconds(3000));
#else
			boost::this_thread::sleep_for(boost::chrono::milliseconds(3000));
#endif
			goto Retry;
		}
	}
}

int Platform::DealDeviceData::setSnmpParameter(void * session, void *& manager_entity, void *& context, void *& vbl, void *& agent_entity, void *& pdu, const DeviceCompareAttribute & dca)
{
	try
	{
		snmp_->makeManagerEntity(session, manager_entity);
		snmp_->makeContext("public", session, context);
		snmp_->makeAgentEntity(session, dca.deviceIp, agent_entity);
		snmp_->makeVbl(session, vbl);
		snmp_->makePdu(session, vbl, pdu);
	}
	catch (const std::exception& e)
	{
		releaseDeviceResource(nullptr, context, vbl, manager_entity, agent_entity, pdu);
		MONITERSERVER_WARN("%s , releaseDeviceResource failed, device id is %s", e.what(), dca.deviceId.c_str());
		return -1;
	}
	return 0; //all OK
}

void Platform::DealDeviceData::releaseDeviceResource(void * session, void * context, void * vbl, void * manager_entity, void * agent_entity, void * pdu)
{
	if (session)
		snmp_->releaseSession(session);
	if (manager_entity)
		snmp_->releaseEntity(manager_entity);
	if (agent_entity)
		snmp_->releaseEntity(agent_entity);
	if (context)
		snmp_->releaseContext(context);
	if (vbl)
		snmp_->releaseVbl(vbl);
	if (pdu)
		snmp_->releasePdu(pdu);
}

void Platform::DealDeviceData::deviceMonitorThread()
{
	otl_connect db;
	otl_connect::otl_initialize(true); // initialize OCI environment,mutiple thread enable,but not safety
	std::ostringstream message;
	message << cm_.dbUser_name << "/" << cm_.db_password << "@" <<
		cm_.db_ip << ":" << cm_.db_port << "/" << cm_.db_name;
Retry:
	try
	{
		std::vector<DeviceCompareAttribute> device_compare_attribute_temp;
		std::vector<DeviceCompareAttribute> add_device_temp;
		std::vector<DeviceCompareAttribute> decrease_device_temp;

		db.rlogon(message.str().c_str());
		while (run_)
		{
			getMessageFromConfigure(CONFIGURE_DIR);
			getDeviceIpFromDB(device_compare_attribute_temp, db);
			auto ret = isDeviceChange(device_compare_attribute_temp, add_device_temp, decrease_device_temp);
			if (ret)//device changed
				setDevcieAndLoadBalancing(add_device_temp, decrease_device_temp);
			for (auto iter_s = load_.begin(); iter_s != load_.end(); ++iter_s)
			{
				auto iter_mutex = load_mutex_.find(*iter_s);
#if _MSC_VER >=1800
				std::lock_guard<std::mutex> lock(*(iter_mutex->second));
#else
				boost::lock_guard<boost::mutex> lock(*(iter_mutex->second));
#endif				
				for (auto iter = (*iter_s)->begin(); iter != (*iter_s)->end(); ++iter)
				{
					auto oid = model_oid_[(*iter).dca.deviceModel];
					auto cleanVbl = [&iter, this](int count)
					{
						for (int i = 0; i < count; ++i)
						{
							int count_vbl = 0;
							snmp_->getCountVbl((*iter).vbl, count_vbl);
							snmp_->deleteVb((*iter).vbl, count_vbl);
						}
					};
					auto sendRequest = [&oid, &iter, &cleanVbl, this](int need_send_count)
					{
						int count = 0;
						for (auto iter_oid = oid.begin(); iter_oid != oid.end(); ++iter_oid) //since together will get response with error tooBig
						{
							snmp_->setVbl((*iter).vbl, 0, iter_oid->second);
							++count;
							if (count == need_send_count)
							{
								++iter_oid;
								oid.erase(oid.begin(), iter_oid);
								break;
							}
						}
						snmp_->setPdu((*iter).pdu, &((*iter).vbl));
						snmp_->sendRequestMessage((*iter).session, (*iter).manager_entity, (*iter).agent_entity,
							(*iter).context, (*iter).pdu);
						cleanVbl(count);
					};
					try
					{
						snmp_->setVbl((*iter).vbl, 0, oid["sysDesc"]); //deal sysDesc alone
						snmp_->setPdu((*iter).pdu, &((*iter).vbl));
						snmp_->sendRequestMessage((*iter).session, (*iter).manager_entity, (*iter).agent_entity,
							(*iter).context, (*iter).pdu);
						cleanVbl(1);
						oid.erase("sysDesc");

						int group = oid.size() / SEND_OID_COUNT;//since together will get response with error tooBig
						if (group == 0)
							sendRequest(oid.size());
						else
						{
							for (int i = 0; i < group; i++)
								sendRequest(SEND_OID_COUNT);
							if (!oid.empty()) //deal left oid
								sendRequest(oid.size());
						}
						is_oid_whole_[(*iter).dca] = true; //a group oid all send out
					}
					catch (const std::exception& e)
					{
						MONITERSERVER_WARN("%s ,sendRequest failed, device id is %s", e.what(), (*iter).dca.deviceId.c_str());
						// do some clean
						int count_vbl = 0;
						snmp_->getCountVbl((*iter).vbl, count_vbl);
						cleanVbl(count_vbl);
						is_oid_whole_[(*iter).dca] = false;
						device_port_flow_[(*iter).dca].clear();
					}
				}
			}
			device_compare_attribute_temp.clear();
			add_device_temp.clear();
			decrease_device_temp.clear();
#if _MSC_VER >=1800
			std::this_thread::sleep_for(std::chrono::milliseconds(cm_.moniter_rate));
#else
			boost::this_thread::sleep_for(boost::chrono::milliseconds(cm_.moniter_rate));
#endif
		}
	}
	catch (otl_exception& p)// intercept OTL exceptions. ex:ORA-12543: TNS:destination host unreachable
	{
		MONITERSERVER_ERROR("%s", p.msg);//error message
		MONITERSERVER_ERROR("%s", p.stm_text);//SQL that caused the error
		MONITERSERVER_ERROR("%s", p.var_info);//the variable that caused the error

		if (db.connected == 1)
			db.logoff();
#if _MSC_VER >=1800
		std::this_thread::sleep_for(std::chrono::milliseconds(3000));
#else
		boost::this_thread::sleep_for(boost::chrono::milliseconds(3000));
#endif
		goto Retry;
	}
	catch (const std::exception& e)
	{
		moniterThread_exception_exit_ = true;
		MONITERSERVER_ERROR("%s, deviceMonitorThread exit", e.what());
	}
	catch (...)
	{
		moniterThread_exception_exit_ = true;
		MONITERSERVER_ERROR("deviceMonitorThread exit with unknown exception!!");
	}
}
