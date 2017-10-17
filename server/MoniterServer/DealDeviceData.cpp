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
#include "mysql.h"
#include "RAII.h"
#include "LogMacros.h"
#include "ParseIni.h"
#include "DbWriter.h"
static std::string gbkToUtf8(const std::string& strGBK)
{
	int n = MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, NULL, 0);
	wchar_t * str1 = new wchar_t[n];
	MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, str1, n);
	n = WideCharToMultiByte(CP_UTF8, 0, str1, -1, NULL, 0, NULL, NULL);
	char * str2 = new char[n];
	WideCharToMultiByte(CP_UTF8, 0, str1, -1, str2, n, NULL, NULL);
	std::string strOutUTF8 = str2;
	delete[]str1;
	str1 = nullptr;
	delete[]str2;
	str2 = nullptr;
	return strOutUTF8;
}

static std::string utf8ToGbk(const std::string& utf8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, wstr, len);
	len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);
	std::string strOutGbk = str;
	delete[] str;
	delete[] wstr;
	return strOutGbk;
}

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
		auto db_write = impl->this_ptr->getDbWriterPtr();
		auto mutex_ptr = impl->mutex_ptr;
		auto snmp_ptr = impl->this_ptr->getSnmp();
		const auto& device_compare_attribute = impl->this_ptr->getDeviceCompareAttribute();
		const auto& model_oid = impl->this_ptr->getModelOid();
		auto& device_data = impl->this_ptr->getDeviceData();
		auto& device_online_flag = impl->this_ptr->getDeviceOnlineFlag();

		std::lock_guard<std::mutex> lock(*mutex_ptr);
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

			device_online_flag[dca] = 1; //recv message OK ,no matter the data is valid or not, the device is onlie
			 //for deviceTable,insert online/offline
			std::ostringstream sql;
			sql << "update " << impl->this_ptr->cm_.deviceTable_name_oracle
				<< " set " << Platform::DEVICE_WORK_STATUS << " = " << "'" << 1 << "'"
				<< " where " << Platform::DEVICE_ID << " = " << "'" << dca.deviceId << "'"
				<< " and " << Platform::DEVICE_IP << " = " << "'" << dca.deviceIp << "'"
				<< " and " << Platform::DEVICE_MODEL << " = " << "'" << dca.deviceModel << "'";
			db_write->addSql(std::move(sql.str()));

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
						char desc[1024 * 2] = { 0 };
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
					is_oid_whole[dca] = false; //this time  is done OK ,make false for next time
				}
			}
		}
		catch (const std::exception& e)
		{
			auto& device_port_flow = impl->this_ptr->getPortflow();
			MONITERSERVER_ERROR("%s, get device data failed, device id(%s),ip(%s) ", e.what(), dca.deviceId.c_str(), dca.deviceIp.c_str());
			device_data[dca].clear();
			device_port_flow[dca].clear();
			is_oid_whole[dca] = false;
			return -1;
		}
		return 0;
	};
	auto writeToDB = [&impl, &dca]()
	{
		auto db_write = impl->this_ptr->getDbWriterPtr();
		auto &device_port_flow = impl->this_ptr->getPortflow();
		auto &device_data = impl->this_ptr->getDeviceData();

		auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		std::ostringstream time;
		time << (std::localtime(&t)->tm_year + 1900) << "-" << (std::localtime(&t)->tm_mon + 1) << "-" << std::localtime(&t)->tm_mday
			<< " " << std::localtime(&t)->tm_hour << ":" << std::localtime(&t)->tm_min << ":" << std::localtime(&t)->tm_sec;
		std::ostringstream sql;
		std::ostringstream sql_value;
		//insert into devicedata(u_number,upload_time) values('22',to_date('2017-7-13 11:50:12','YYYY-MM-DD HH24:MI:SS'))
		sql << "insert into " << impl->this_ptr->cm_.deviceDataTable_name_oracle << "(" << Platform::DEVICE_ID
			<< "," << Platform::DEVICE_UPLOAD_TIME;
		sql_value << "values (" << "'" << dca.deviceId << "'" << "," << "to_date('" << time.str() << "','YYYY-MM-DD HH24:MI:SS')";

		auto iter_sys_desc = device_data[dca].find("sysDesc");
		if (iter_sys_desc != device_data[dca].end()) // has system_desc
		{
			sql << "," << Platform::DEVICE_SYSTEM_DESCRIPTION;
			sql_value << "," << "'" << (std::any_cast<std::string>(iter_sys_desc->second)) << "'";
			device_data[dca].erase(iter_sys_desc); //remove this element
		}

		auto iter_cpu = device_data[dca].find("cpu");
		if (iter_cpu != device_data[dca].end())//just has single cpu
		{
			sql << "," << Platform::DEVICE_CPU_RATE;
			sql_value << "," << "'" << std::any_cast<uint64_t>(iter_cpu->second) << "'";
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
						exit_loop = true; //since sort , once goto else branch,then no cpux exit
						cpu_rate_all = cpu_rate_all + std::any_cast<uint64_t>(iter->second);
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
			auto diskTotal = std::any_cast<uint64_t>(iter_diskTotal->second);
			device_data[dca].erase(iter_diskTotal); //remove this element
			auto iter_diskUsed = device_data[dca].find("diskUsed");
			if (iter_diskUsed != device_data[dca].end())
			{
				auto diskUsed = std::any_cast<uint64_t>(iter_diskUsed->second);
				device_data[dca].erase(iter_diskUsed); //remove this element
				if (diskTotal != 0)
				{
					auto disk_use_rate = diskUsed * 100 / diskTotal;
					sql << "," << Platform::DEVICE_DISK_RATE;
					sql_value << "," << "'" << disk_use_rate << "'";
				}
				else
					MONITERSERVER_WARN("device(%s) has single disk,but diskTotal is 0, so do not insert disk_use_rate", dca.deviceIp.c_str());
			}
		}
		else //has several disks or no disk
		{
			uint64_t diskTotal_all = 0;
			uint64_t diskUsed_all = 0;
			bool exit_loop = false;
			bool any_diskTotal_isZero = false;

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
							//diskTotal.emplace_back(std::any_cast<uint64_t>(iter->second));
							auto diskTotal_temp = std::any_cast<uint64_t>(iter->second);
							if (diskTotal_temp != 0)
								diskTotal_all = diskTotal_all + std::any_cast<uint64_t>(iter->second);
							else
							{
								any_diskTotal_isZero = true;
								MONITERSERVER_WARN("device(%s) one diskTotal is 0, so do not insert disk_use_rate", dca.deviceIp.c_str());
							}
							iter = device_data[dca].erase(iter);
						}
						else //find one like diskUsedx(x=1 or 2 or 3.......)
						{
							//diskUsed.emplace_back(std::any_cast<uint64_t>(iter->second));
							diskUsed_all = diskUsed_all + std::any_cast<uint64_t>(iter->second);
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
				if (!any_diskTotal_isZero)
				{
					auto disk_use_rate = diskUsed_all * (float)100.0 / diskTotal_all;
					sql << "," << Platform::DEVICE_DISK_RATE;
					sql_value << "," << "'" << disk_use_rate << "'";
				}
			}
		}

		auto iter_memory = device_data[dca].find("memory");
		if (iter_memory != device_data[dca].end()) //do not need calculate,direct get
		{
			auto mem_use_rate = std::any_cast<uint64_t>(iter_memory->second);
			device_data[dca].erase(iter_memory); //remove this element
			sql << "," << Platform::DEVICE_MEMORY_RATE;
			sql_value << "," << "'" << mem_use_rate << "'";
		}
		else //need calculate
		{
			auto iter_memoryTotal = device_data[dca].find("memoryTotal");
			if (iter_memoryTotal != device_data[dca].end()) //has memory
			{
				auto memoryTotal = std::any_cast<uint64_t>(iter_memoryTotal->second);
				device_data[dca].erase(iter_memoryTotal); //remove this element
				auto iter_memoryUsed = device_data[dca].find("memoryUsed");
				if (iter_memoryUsed != device_data[dca].end())
				{
					auto memoryUsed = std::any_cast<uint64_t>(iter_memoryUsed->second);
					device_data[dca].erase(iter_memoryUsed); //remove this element
					if (memoryTotal != 0)
					{
						auto mem_use_rate = memoryUsed * (float)100.0 / memoryTotal;
						sql << "," << Platform::DEVICE_MEMORY_RATE;
						sql_value << "," << "'" << mem_use_rate << "'";
					}
					else
						MONITERSERVER_WARN("device(%s) memoryTotal is 0,so do not insert mem_use_rate", dca.deviceIp.c_str());
				}
			}
		}

		auto iter_temperature = device_data[dca].find("temperature");
		if (iter_temperature != device_data[dca].end())// has temperature
		{
			sql << "," << Platform::DEVICE_CPU_TEMPERATURE;
			sql_value << "," << "'" << (std::any_cast<uint64_t>(iter_temperature->second)) << "'";
			device_data[dca].erase(iter_temperature); //remove this element
		}

		// at last, just leave net
		int port_count = device_data[dca].size() / 3;
		if (port_count > 0)
		{
			bool any_ifSpeed_isZero = false;
			std::vector<Platform::PortFlow> this_port_flow;
			Platform::PortFlow port_flow;
			auto iter_net = device_data[dca].begin();
			for (int i = 0; i < port_count; ++i, ++iter_net)
			{
				auto iter_net_temp = iter_net;
				port_flow.ifSpeed = static_cast<int32_t>(std::any_cast<uint64_t>(iter_net_temp->second));
				if (port_flow.ifSpeed == 0)
				{
					MONITERSERVER_WARN("device(%s) ifSpeed which get from device is 0,so do not insert net_use_rate", dca.deviceIp.c_str());
					any_ifSpeed_isZero = true;
					break;
				}

				for (int j = 0; j < port_count; ++j)
					++iter_net_temp;
				port_flow.in = static_cast<int32_t>(std::any_cast<uint64_t>(iter_net_temp->second));

				for (int p = 0; p < port_count; ++p)
					++iter_net_temp;
				port_flow.out = static_cast<int32_t>(std::any_cast<uint64_t>(iter_net_temp->second));
				this_port_flow.emplace_back(port_flow);
			}

			auto iter = device_port_flow.find(dca);
			if (!any_ifSpeed_isZero)
			{
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
						MONITERSERVER_WARN("the device(ip:%s) maybe reboot,net_useRate_average is %f", dca.deviceIp.c_str(), net_useRate_average);

					auto iter_max = std::max_element(net_use_rate.begin(), net_use_rate.end());
					sql << "," << Platform::DEVICE_PORT_BANDWIDTH_MAX;
					sql_value << "," << "'" << *iter_max << "'";
					sql << "," << Platform::DEVICE_BANDWIDTH_USE_AVERAGE;
					sql_value << "," << "'" << net_useRate_average << "'";

					iter->second.swap(this_port_flow);//this_port_flow will become the last_port_flow for the next request
				}
				else // no last port_flow data,this time do not write db
					device_port_flow[dca] = this_port_flow;
			}
			else
				iter->second.clear(); // for next request ,there is no last_port_flow any more
		}
		device_data[dca].clear();

		sql << "," << Platform::DEVICE_STATUS;
		sql_value << "," << "'" << 1 << "'"; //the device is online
		sql << ")";
		sql_value << ")";
		sql << sql_value.str();
		auto str = sql.str();
		db_write->addSql(std::move(sql.str()));

		//end = std::chrono::system_clock::now();
		//int elapsed_seconds = std::chrono::duration_cast<std::chrono::milliseconds>
		//	(end - start).count();
	};

	try
	{
		auto ret = getDeviceData();
		if (ret != 0) // get device date failed
			return (SNMPAPI_STATUS)-1;
		if (all_get)
		{
			all_get = false;
			writeToDB();
		}
	}
	catch (const std::exception& e)
	{
		MONITERSERVER_ERROR("%s in snmpCallBack", e.what());
	}
	catch (...)
	{
		MONITERSERVER_ERROR("unknown exception in snmpCallBack");
	}

	/*auto end1 = std::chrono::system_clock::now();
		int elapsed_seconds1 = std::chrono::duration_cast<std::chrono::milliseconds>
			(end1 - start).count();
		printf("%d\n", elapsed_seconds1);*/
	return 0;
}

Platform::DealDeviceData::DealDeviceData() :run_(true), device_count_(0),
snmp_(std::make_shared<Snmp>(Snmp())),
moniterThread_exception_exit_(false)
{}

void Platform::DealDeviceData::init()
{
	getMessageFromIniConfigure(INI_CONFIGURE_DIR);
	std::ostringstream message;
	message << cm_.dbUser_name_oracle << "/" << cm_.db_password_oracle << "@" <<
		cm_.db_ip_oracle << ":" << cm_.db_port_oracle << "/" << cm_.db_name_oracle;

	db_writer_ = std::make_shared<WwFoundation::DbWriter>(1, 2000000, 1, message.str()); //max_sql_count:2000000 nearly 1G ,if each sql 512Byte
	load_.resize(cm_.load_count);
	snmp_->init();
	makeLoadsession();
	//thread_moniter_ = std::thread(std::bind(&DealDeviceData::deviceMonitorThread, this));
	//thread_moniter_offline_ = std::thread(std::bind(&DealDeviceData::deviceOfflineMonitorThread, this));
	thread_monitor_camera_status_ = std::thread(std::bind(&DealDeviceData::cameraStatusMonitorThread, this));
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

int Platform::DealDeviceData::getDbWriterThreadAliveCount() const
{
	return db_writer_->getThreadAliveCount();
}

void Platform::DealDeviceData::getMessageFromIniConfigure(const std::string& dir)
{
	WwFoundation::ParseIni parser(dir);
	parser.parse();
	cm_.dbUser_name_oracle = parser.getValue("OracleDbConfig", "DbUserName");
	cm_.db_ip_oracle = parser.getValue("OracleDbConfig", "DbIp");
	cm_.db_name_oracle = parser.getValue("OracleDbConfig", "DbName");
	cm_.db_password_oracle = parser.getValue("OracleDbConfig", "DbPassword");
	cm_.db_port_oracle = parser.getValue("OracleDbConfig", "DbPort");
	cm_.deviceDataTable_name_oracle = parser.getValue("OracleDbConfig", "DeviceDataTableName");
	cm_.deviceTable_name_oracle = parser.getValue("OracleDbConfig", "DeviceTableName");
	cm_.device_alarmLogTable_name_oracle= parser.getValue("OracleDbConfig", "DeviceAlarmLogTableName");
	cm_.load_count = std::atoi(parser.getValue("ComConfig", "LoadCount").c_str());
	cm_.moniter_rate = std::atoi(parser.getValue("ComConfig", "MoniterRate").c_str());
	//cm_.moniter_rate = cm_.moniter_rate >= 30000 ? cm_.moniter_rate : 30000; //snmp delay is nearly 30s

	cm_.dbUser_name_mysql = parser.getValue("MysqlDbConfig", "DbUserName");
	cm_.db_ip_mysql = parser.getValue("MysqlDbConfig", "DbIp");
	cm_.db_name_mysql = parser.getValue("MysqlDbConfig", "DbName");
	cm_.db_password_mysql = parser.getValue("MysqlDbConfig", "DbPassword");
	cm_.db_port_mysql = parser.getValue("MysqlDbConfig", "DbPort");
	cm_.cameraTable_name_mysql = parser.getValue("MysqlDbConfig", "DeviceTableName");
}

void Platform::DealDeviceData::getMessageFromConfigure(const std::string& dir)
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
		<< cm_.deviceTable_name_oracle << " where " << DEVICE_MODEL << " in('";
	auto size = device_model_name.size();
	for (auto i = 0; i < static_cast<int>(size - 1); ++i)
		message << device_model_name.at(i) << "','";
	message << device_model_name.at(size - 1) << "')" << " and " << DEV_STATUS_NORMAL << " = '101'";
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

void Platform::DealDeviceData::setDevcieAndLoadBalancing(std::vector<DeviceCompareAttribute>& add, const std::vector<DeviceCompareAttribute>& decrease)
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
					//memset(&da, 0, sizeof(DeviceAttribute));
					da.dca = *iter;
					da.session = load_session_[*iter_s];
					auto ret = setSnmpParameter(da.session, da.manager_entity, da.context, da.vbl,
						da.agent_entity, da.pdu, *iter);
					if (ret == 0)//setSnmpParameter ok
					{
						(*iter_s)->emplace_back(da);
						device_port_flow_[*iter];//make empty value,make sure the key exists when using
						device_data_[*iter];//make empty value
						device_online_flag_[*iter];
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
				da.dca = (*iter);
				da.session = load_session_[*iter_s];
				auto ret = setSnmpParameter(da.session, da.manager_entity, da.context, da.vbl,
					da.agent_entity, da.pdu, *iter);
				if (ret == 0)//setSnmpParameter ok
				{
					(*iter_s)->emplace_back(da);
					++iter_s;
					device_port_flow_[da.dca];//make empty value,make sure the key exists when using
					device_data_[da.dca];//make empty value
					device_online_flag_[da.dca];
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
						device_port_flow_.erase((*iter).dca);// note: if the device_port_flow_ has been read at the same time in the function(snmpCallBack,another thread),this will make undefined behavior. But it will not happen under the large moniter_rate
						device_data_.erase((*iter).dca); //as same as above
						is_oid_whole_.erase(iter->dca);// as same as above
						device_online_flag_.erase(iter->dca); // be read in deviceOfflineMonitorThread,also as same as above
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
	for (int i = 0; i < cm_.load_count; ++i)
	{
		auto impl = std::make_shared<Impl>(Impl());
		impl->this_ptr = shared_this;
		auto mutex_ptr = std::shared_ptr<std::mutex>(new std::mutex);
		impl->mutex_ptr = mutex_ptr;
		void* session;
		snmp_->makeSession(snmpCallBack, impl.get(), session);

		auto ptr = std::make_shared<std::vector<DeviceAttribute>>(std::vector<DeviceAttribute>());
		load_[i] = ptr;
		load_session_.emplace(std::make_pair(ptr, session));
		load_mutex_.emplace(std::make_pair(ptr, mutex_ptr));
		impl_.emplace_back(impl);
	}
}

int Platform::DealDeviceData::setSnmpParameter(void * session, void *& manager_entity, void *& context, void *& vbl, void *& agent_entity, void *& pdu, const DeviceCompareAttribute & dca)
{
	try
	{
		snmp_->makeManagerEntity(session, manager_entity);
		WwFoundation::RAII makeManagerEntity_raii([manager_entity, this]() {snmp_->releaseEntity(manager_entity); });

		snmp_->makeContext("public", session, context);
		WwFoundation::RAII makeContext_raii([context, this]() {snmp_->releaseContext(context); });

		snmp_->makeAgentEntity(session, dca.deviceIp, agent_entity);
		WwFoundation::RAII makeAgentEntity_raii([agent_entity, this]() {snmp_->releaseEntity(agent_entity); });

		snmp_->makeVbl(session, vbl);
		WwFoundation::RAII makeVbl_raii([vbl, this]() {snmp_->releaseVbl(vbl); });

		snmp_->makePdu(session, vbl, pdu);
		WwFoundation::RAII makePdu_raii([pdu, this]() {snmp_->releasePdu(pdu); });

		makeManagerEntity_raii.Dismiss();
		makeContext_raii.Dismiss();
		makeAgentEntity_raii.Dismiss();
		makeVbl_raii.Dismiss();
		makePdu_raii.Dismiss();
	}
	catch (const std::exception& e)
	{
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

void Platform::DealDeviceData::deviceMonitorThread() noexcept
{
	otl_connect db;
	try
	{
		otl_connect::otl_initialize(true); // initialize OCI environment,mutiple thread enable,but not safety
		std::ostringstream message;
		message << cm_.dbUser_name_oracle << "/" << cm_.db_password_oracle << "@" <<
			cm_.db_ip_oracle << ":" << cm_.db_port_oracle << "/" << cm_.db_name_oracle;

		bool need_rlogon = true;
		while (run_)
		{
			std::vector<DeviceCompareAttribute> device_compare_attribute_temp;
			try
			{
				if (need_rlogon)
				{
					db.rlogon(message.str().c_str());
					need_rlogon = false;
				}
				getMessageFromConfigure(CONFIGURE_DIR);
				getDeviceIpFromDB(device_compare_attribute_temp, db);
			}
			catch (otl_exception& p)// intercept OTL exceptions. ex:ORA-12543: TNS:destination host unreachable
			{
				MONITERSERVER_ERROR("%s", p.msg);//error message
				MONITERSERVER_ERROR("%s", p.stm_text);//SQL that caused the error
				MONITERSERVER_ERROR("%s", p.var_info);//the variable that caused the error

				if (db.connected == 1)
					db.logoff();
				std::this_thread::sleep_for(std::chrono::milliseconds(retry_rate));
				need_rlogon = true;
				continue;
			}
			std::vector<DeviceCompareAttribute> add_device_temp;
			std::vector<DeviceCompareAttribute> decrease_device_temp;
			auto ret = isDeviceChange(device_compare_attribute_temp, add_device_temp, decrease_device_temp);
			if (ret)//device changed
				setDevcieAndLoadBalancing(add_device_temp, decrease_device_temp);
			for (auto iter_s = load_.begin(); iter_s != load_.end(); ++iter_s)
			{
				auto iter_mutex = load_mutex_.find(*iter_s);
				std::lock_guard<std::mutex> lock(*(iter_mutex->second));

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
						if (count != 0)
						{
							snmp_->setPdu((*iter).pdu, &((*iter).vbl));
							snmp_->sendRequestMessage((*iter).session, (*iter).manager_entity, (*iter).agent_entity,
								(*iter).context, (*iter).pdu);
							cleanVbl(count);
						}
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
			std::this_thread::sleep_for(std::chrono::milliseconds(cm_.moniter_rate));
		}
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

void Platform::DealDeviceData::deviceOfflineMonitorThread() noexcept
{
	try
	{
		while (run_)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(cm_.moniter_rate / 2));
			for (auto& device_online_flag_pair : device_online_flag_)
			{
				if (device_online_flag_pair.second == 0)// the device maybe offline
				{
					auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
					std::ostringstream time;
					time << (std::localtime(&t)->tm_year + 1900) << "-" << (std::localtime(&t)->tm_mon + 1) << "-" << std::localtime(&t)->tm_mday
						<< " " << std::localtime(&t)->tm_hour << ":" << std::localtime(&t)->tm_min << ":" << std::localtime(&t)->tm_sec;

					//for device data table
					std::ostringstream sql;
					sql << "insert into " << cm_.deviceDataTable_name_oracle << "(" << Platform::DEVICE_ID
						<< "," << Platform::DEVICE_UPLOAD_TIME << "," << Platform::DEVICE_STATUS << ")"
						<< " values (" << "'" << device_online_flag_pair.first.deviceId << "'" << ","
						<< "to_date('" << time.str() << "','YYYY-MM-DD HH24:MI:SS')" << ","
						<< "'" << DEVICE_OFFLINE <<"'" << ")";
					db_writer_->addSql(std::move(sql.str()));

					//for deviceTable,insert offline
					sql.clear();
					sql.str("");
					sql << "update " << cm_.deviceTable_name_oracle
						<< " set " << Platform::DEVICE_WORK_STATUS << " = " << "'" << DEVICE_OFFLINE << "'"
						<< " where " << Platform::DEVICE_ID << " = " << "'" << device_online_flag_pair.first.deviceId << "'"
						<< " and " << Platform::DEVICE_IP << " = " << "'" << device_online_flag_pair.first.deviceIp << "'"
						<< " and " << Platform::DEVICE_MODEL << " = " << "'" << device_online_flag_pair.first.deviceModel << "'";
					db_writer_->addSql(std::move(sql.str()));
				}
				device_online_flag_pair.second = 0;
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(cm_.moniter_rate / 2));
		}
	}
	catch (const std::exception& e)
	{
		MONITERSERVER_ERROR("%s, deviceOfflineMonitorThread exit", e.what());
	}
	catch (...)
	{
		MONITERSERVER_ERROR("deviceOfflineMonitorThread exit with unknown exception!!");
	}
}

void Platform::DealDeviceData::cameraStatusMonitorThread() noexcept
{
	typedef struct cameraipport
	{
		std::string ip;
		uint16_t port;
		cameraipport(const std::string& Ip, uint16_t Port) :ip(Ip), port(Port) {}
		bool operator ==(const cameraipport cip) const
		{
			return (ip == cip.ip && port == cip.port);
		}
	} CameraIpPort;
	struct CameraIpPortHash
	{
		std::size_t operator()(const CameraIpPort& k) const
		{
			auto h1 = std::hash<std::string>()(k.ip);
			auto h2 = std::hash<uint16_t>()(k.port);
			return (h2 << 1) ^ (h1 >> 1); // or use boost::hash_combine
		}
	};
	
	try
	{
		otl_connect db;
		otl_connect::otl_initialize(true); // initialize OCI environment,mutiple thread enable,but not safety
		std::ostringstream oracle_message;
		oracle_message << cm_.dbUser_name_oracle << "/" << cm_.db_password_oracle << "@" <<
			cm_.db_ip_oracle << ":" << cm_.db_port_oracle << "/" << cm_.db_name_oracle;

		std::ostringstream message;
		message << "select " << DEVICE_IP << "," << DEVICE_PORT <<  " from "
			<< cm_.deviceTable_name_oracle << " where " << ITS_DEVICE_TYPE << " = " << "'" << CAMERA << "'"
			<< " and " << DEV_STATUS_NORMAL << " = " << "'" << DEVICE_STATUS_OK << "'";
		auto oracle_sql = message.str();

		auto t_mysql = mysql_init(nullptr);
		if (!t_mysql)
		{
			std::ostringstream exception_message;
			exception_message << "mysql_init failed with:" << mysql_error(t_mysql);
			throw std::runtime_error(exception_message.str());
		}
		message.clear();
		message.str("");
		message << "select " << CAMERA_IPV4 << "," << CAMERA_PORT << "," << CAMERA_STATUS_CODE
			<< " from " << cm_.cameraTable_name_mysql;
		auto mysql_sql = message.str();

		bool need_rlogon_oracle = true;
		bool need_rlogon_mysql = true;
		MYSQL *connect = nullptr;
		std::unordered_map<CameraIpPort, std::string, CameraIpPortHash> camera_status_code_last;
		while (run_)
		{
			std::vector<CameraIpPort> its_device_camera_ip_port;
			std::ostringstream update_sql;
			std::ostringstream insert_sql;
			try
			{
				if (need_rlogon_oracle)
				{
					db.rlogon(oracle_message.str().c_str());
					need_rlogon_oracle = false;
				}
				otl_stream o(50, oracle_sql.c_str(), db);//otl_stream just for select, else use otl_nocommit_stream, since select do not need to commit
				std::string device_ip, device_code = "";
				int device_port = 0;
				while (!o.eof())
				{
					o >> device_ip >> device_port;
					CameraIpPort ip_port{ "",0 };
					ip_port.ip = device_ip;
					ip_port.port = static_cast<uint16_t>(device_port);
					its_device_camera_ip_port.emplace_back(ip_port);
				}
				o.close(true); //reuse this stream 
			}
			catch (otl_exception& p)// intercept OTL exceptions. ex:ORA-12543: TNS:destination host unreachable
			{
				MONITERSERVER_ERROR("%s", p.msg);//error message
				MONITERSERVER_ERROR("%s", p.stm_text);//SQL that caused the error
				MONITERSERVER_ERROR("%s", p.var_info);//the variable that caused the error
				if (db.connected == 1)
					db.logoff();
				std::this_thread::sleep_for(std::chrono::milliseconds(retry_rate));
				need_rlogon_oracle = true;
				continue;
			}
		
			while (run_)
			{
				if (need_rlogon_mysql)
				{
					connect = mysql_real_connect(t_mysql, cm_.db_ip_mysql.c_str(), cm_.dbUser_name_mysql.c_str(),
						cm_.db_password_mysql.c_str(), cm_.db_name_mysql.c_str(), std::atoi(cm_.db_port_mysql.c_str()), NULL, 0);
					if (!connect)
					{
						MONITERSERVER_ERROR("mysql_real_connect failed :%s", mysql_error(connect));
						continue;
					}
					need_rlogon_mysql = false;
				}
				auto ret = mysql_real_query(connect, mysql_sql.c_str(), mysql_sql.length());
				if (ret)
				{
					MONITERSERVER_ERROR("mysql_real_query failed:%s", mysql_error(connect));
					mysql_close(connect);
					need_rlogon_mysql = true;
					continue;
				}
				auto mysql_result = mysql_store_result(connect);
				auto num_row = mysql_num_rows(mysql_result);
				auto num_col = mysql_num_fields(mysql_result);
				if (num_col == 3)
				{
					auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
					std::ostringstream time;
					time << (std::localtime(&t)->tm_year + 1900) << "-" << (std::localtime(&t)->tm_mon + 1) << "-" << std::localtime(&t)->tm_mday
						<< " " << std::localtime(&t)->tm_hour << ":" << std::localtime(&t)->tm_min << ":" << std::localtime(&t)->tm_sec;

					std::unordered_map<CameraIpPort, std::string, CameraIpPortHash> camera_status_code_this;
					for (int i = 0; i < num_row; ++i)
					{
						update_sql.clear();
						update_sql.str("");
						auto mysql_row = mysql_fetch_row(mysql_result); //mysql_row[0]--ip, mysql_row[1]--port,mysql_row[2]--status_code
						camera_status_code_this.emplace(CameraIpPort(mysql_row[0], static_cast<uint16_t>(std::atoi(mysql_row[1]))), mysql_row[2]);
						if (mysql_row[2]== CAMERA_OFFLINE) //camera offline
						{
							update_sql << "update " << cm_.deviceTable_name_oracle << " set " << DEVICE_WORK_STATUS
								<< " = " << "'" << DEVICE_OFFLINE << "'" << " where "
								<< DEVICE_IP << " = " << "'" << mysql_row[0] << "'"
								<< " and " << DEVICE_PORT << " = " << "'" << mysql_row[1] << "'";
							db_writer_->addSql(update_sql.str());
						}
						else if (mysql_row[2]== CAMERA_ONLINE) //camera online
						{
							update_sql << "update " << cm_.deviceTable_name_oracle << " set " << DEVICE_WORK_STATUS
								<< " = " << "'" << DEVICE_ONLINE << "'" << " where "
								<< DEVICE_IP << " = " << "'" << mysql_row[0] << "'"
								<< " and " << DEVICE_PORT << " = " << "'" << mysql_row[1] << "'";
							db_writer_->addSql(update_sql.str());
						}
						else //camera record video now ,must be online
						{
							update_sql << "update " << cm_.deviceTable_name_oracle << " set " << DEVICE_WORK_STATUS
								<< " = " << "'" << CAMERA_ON_VIDEO << "'" << " where "
								<< DEVICE_IP << " = " << "'" << mysql_row[0] << "'"
								<< " and " << DEVICE_PORT << " = " << "'" << mysql_row[1] << "'";
							db_writer_->addSql(update_sql.str());
						}
					}
					//compare
					for (auto iter_this = camera_status_code_this.begin(); iter_this != camera_status_code_this.end(); ++iter_this)
					{
						auto iter_last = std::find(camera_status_code_last.begin(), camera_status_code_last.end(), *iter_this);
						if (iter_last != camera_status_code_last.end()) //both has, no change
							camera_status_code_last.erase(iter_last);
						else //maybe new camera or maybe status changed
						{
							auto iter = std::find(its_device_camera_ip_port.begin(), its_device_camera_ip_port.end(), iter_this->first);
							if (iter != its_device_camera_ip_port.end()) // oracle its_device has this camera, record alarm log
							{
								insert_sql.clear();
								insert_sql.str("");
								insert_sql << "insert into " << cm_.device_alarmLogTable_name_oracle << "(" << ALARM_TIME
									<< "," << ALARM_DEAL_STATUS << "," << ALARM_TYPE << "," << ALARM_LEVEL
									<< "," << ALARM_DETAIL << ")"
									<< " values(" << "to_date('" << time.str() << "','YYYY-MM-DD HH24:MI:SS')"
									<< "," << "'" << ALARM_DEAL_STATUS_NO_DEAL << "'";
								if (iter_this->second == CAMERA_OFFLINE) //camera turn offline
								{
									std::string alarm_detail = "摄相机断线";
									auto alarm_detail_utf8 = gbkToUtf8(alarm_detail);
									insert_sql << "," << "'" << ALARM_TYPE_OFFLINE << "'"
										<< "," << "'" << ALARM_LEVEL_FATAL << "'"
										<< "," << "'" << iter_this->first.ip << ":" << iter_this->first.port
										<< " " << alarm_detail_utf8 << "'" << ")";
									db_writer_->addSql(insert_sql.str());
								}
								else //camera online （maybe turn online or maybe always online ）
								{
									auto iter_last1 = camera_status_code_last.find(CameraIpPort(iter_this->first.ip, iter_this->first.port));
									if (iter_last1 != camera_status_code_last.end()) //find ,means status changed
									{
										if (iter_last1->second == CAMERA_OFFLINE)// last time offline
										{
											std::string alarm_detail = "摄相机在线";
											auto alarm_detail_utf8 = gbkToUtf8(alarm_detail);
											insert_sql << "," << "'" << ALARM_TYPE_ONLINE << "'"
												<< "," << "'" << ALARM_LEVEL_ORDINARY << "'"
												<< "," << "'" << iter_this->first.ip << ":" << iter_this->first.port
												<< " " << alarm_detail_utf8 << "'" << ")";
											db_writer_->addSql(insert_sql.str());
										}
									}
									else // not find ,means new camera
									{
										std::string alarm_detail = "摄相机在线";
										auto alarm_detail_utf8 = gbkToUtf8(alarm_detail);
										insert_sql << "," << "'" << ALARM_TYPE_ONLINE << "'"
											<< "," << "'" << ALARM_LEVEL_ORDINARY << "'"
											<< "," << "'" << iter_this->first.ip << ":" << iter_this->first.port
											<< " " << alarm_detail_utf8 << "'" << ")";
										db_writer_->addSql(insert_sql.str());
									}
								}
							}
							else
								MONITERSERVER_WARN("camera(%s:%d) status_code:%s is not in its_device,do not record alarm log", iter_this->first.ip.c_str(), iter_this->first.port, iter_this->second.c_str());
						}
					}
					camera_status_code_last = camera_status_code_this;//change last status
				}
				else
					MONITERSERVER_WARN("get %d colnums data from mysql rather than 3 colnums", num_col);
				mysql_free_result(mysql_result);
				break;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(cm_.moniter_rate));
		}
	}
	catch (const std::exception& e)
	{
		MONITERSERVER_ERROR("%s, cameraStatusMonitorThread exit", e.what());
	}
	catch (...)
	{
		MONITERSERVER_ERROR("cameraStatusMonitorThread exit with unknown exception!!");
	}
}