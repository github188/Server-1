#include "CameraManager.h"
#include "JieMai.h" // stay here just for complile
#include <cstdint>
#include <sstream>
#include <algorithm>
#include <memory>
#define OTL_ORA12C_R2 // Compile OTL 4.0/OCI12
#define OTL_STL // Enable STL compatibility mode
#define OTL_ORA_UTF8
#define OTL_STREAM_POOLING_ON
#define OTL_ORA_TIMESTAMP
#include "otlv4_h2/otlv4.h"
#include "KeDa.h"

#include "LogMacros.h"
#include "DbWriter.h"
#include "ThreadPool.h"
#include "ParseIni.h"

void ITS::CameraManager::init()
{
	getMessageFromIniConfigure(INI_CONFIGURE_DIR);
	notify_camera_changed_thread_ = std::thread(std::bind(&CameraManager::notifyCameraChangedThread, this));
}

void ITS::CameraManager::getMessageFromIniConfigure(const std::string & dir)
{
	WwFoundation::ParseIni parser(dir);
	parser.parse();
	cm_.db_UserName = parser.getValue("DbConfig", "DbUserName");
	cm_.db_ip = parser.getValue("DbConfig", "DbIp");
	cm_.db_name = parser.getValue("DbConfig", "DbName");
	cm_.db_password = parser.getValue("DbConfig", "DbPassword");
	cm_.db_port = parser.getValue("DbConfig", "DbPort");
	cm_.videoDetectDataTable_name = parser.getValue("DbConfig", "VideoDetectDataTableName");
	cm_.videoDetectDataTable_name_history = parser.getValue("DbConfig", "VideoDetectDataTableHistoryName");
	cm_.deviceTable_name = parser.getValue("DbConfig", "DeviceTableName");
	cm_.thread_num = std::atoi(parser.getValue("ComConfig", "ThreadNum").c_str());
	cm_.thread_num = cm_.thread_num >= 1 ? cm_.thread_num : 1;
	cm_.keda_userName = parser.getValue("ComConfig", "KeDaUserName");
	cm_.keda_password = parser.getValue("ComConfig", "KeDaPassword");
	cm_.jiemai_userName = parser.getValue("ComConfig", "JieMaiUserName");
	cm_.jiemai_password = parser.getValue("ComConfig", "JieMaiPassword");
	cm_.keda_MaxNumConnect = std::atoi(parser.getValue("ComConfig", "KeDaMaxNumConnect").c_str());
	cm_.sql_commit_size = std::atoi(parser.getValue("ComConfig", "SqlCommitSize").c_str());
	cm_.sql_commit_size = cm_.sql_commit_size >= 1 ? cm_.sql_commit_size : 1;
}

void ITS::CameraManager::getCameraAttributeFromDB(std::vector<CameraAttribute>& camera_attribute, otl_connect & db)
{
	std::ostringstream sql;
	sql << "select " << DEVICE_ID << "," << DEVICE_IP << "," << DEVICE_PORT
		<< "," << DEVICE_SUB_TYPE << "," << DEVICE_LANE_NUMBER
		<< " from " << cm_.deviceTable_name << " where " << ITS_DEVICE_TYPE << " = " << VIDEO_DETECT
		<< " and " << DEVICE_STATUS_NORMAL << " = " << DEVICE_STATUS_OK << " and " << DEVICE_SUB_TYPE
		<< " in( " << "'" << KEDA << "'" << "," << "'" << JIEMAI << "'" << ")";

	otl_stream o(50, sql.str().c_str(), db);//otl_stream just for select, else use otl_nocommit_stream, since select do not need to commit
	std::string device_id, device_ip, device_sub_type, device_lane_number = "";
	int device_port = 0;
	while (!o.eof())
	{
		o >> device_id >> device_ip >> device_port >> device_sub_type >> device_lane_number;
		CameraAttribute ca;
		ca.device_id = device_id;
		ca.ip = device_ip;
		ca.camera_vender = device_sub_type;
		ca.port = static_cast<uint16_t>(device_port);
		if (device_sub_type == KEDA) //keda
			if (ca.port == 80)
				ca.port = 20000;
		auto pos = device_lane_number.find_first_of(EXCISION);
		while (pos != std::string::npos) // has more than one lane
		{
			auto str = device_lane_number.substr(0, pos);
			ca.lane_number.emplace_back(str);
			device_lane_number = device_lane_number.erase(0, pos + 1);
			pos = device_lane_number.find_first_of(EXCISION);
		}
		ca.lane_number.emplace_back(device_lane_number);

		camera_attribute.emplace_back(ca);
	}
	o.close(true); //reuse this stream 
}

bool ITS::CameraManager::isCameraChange(std::vector<CameraAttribute>& camera_attribute,
	std::vector<CameraAttribute>& camera_add, std::vector<CameraAttribute>& camera_decrease)
{
	if (camera_attribute_ == camera_attribute)
		return false;
	else
	{
		if (!camera_attribute_.empty())
		{
			//std::for_each(camera_attribute.begin(), camera_attribute.end(), []() {});
			for (auto iter = camera_attribute.begin(); iter != camera_attribute.end(); ++iter)
			{
				auto iter_ = std::find(camera_attribute_.begin(), camera_attribute_.end(), *iter);
				if (iter_ != camera_attribute_.end()) //both has this element
					camera_attribute_.erase(iter_);
				else //has added device
					camera_add.emplace_back(*iter);
			}
			if (!camera_attribute_.empty())//has decreasing device
				camera_decrease.swap(camera_attribute_);
			camera_attribute_.swap(camera_attribute);
		}
		else
		{
			camera_add = camera_attribute;
			camera_attribute_.swap(camera_attribute);
		}

		CameraIpPort cip("", 0);
		std::unordered_map<CameraIpPort, std::string, CameraIpPortHash> camera_id;
		std::unordered_map<CameraIpPort, std::vector<std::string>, CameraIpPortHash> camera_lane_number;
		for (auto iter = camera_attribute_.begin(); iter != camera_attribute_.end(); ++iter)
		{
			cip.ip = iter->ip;
			cip.port = iter->port;
			camera_id.emplace(cip, iter->device_id);
			camera_lane_number.emplace(cip, iter->lane_number);
		}
		std::unique_lock<std::mutex> locker(number_mtx_);
		camera_number_.swap(camera_id);
		camera_lane_number_.swap(camera_lane_number);
		locker.unlock();
		return true;
	}
}

void ITS::CameraManager::notifyCameraChangedThread() noexcept
{
	try
	{
		std::ostringstream message;
		message << cm_.db_UserName << "/" << cm_.db_password << "@" <<
			cm_.db_ip << ":" << cm_.db_port << "/" << cm_.db_name;
		otl_connect::otl_initialize(true); // initialize OCI environment,mutiple thread enable,but not safety
		auto db_ptr = std::make_shared<otl_connect>();
		auto dbWriter_ptr = std::make_shared<WwFoundation::DbWriter>(DB_WRITER_THREAD_NUMBER, DB_WRITER_MAX_SQL_COUNT, cm_.sql_commit_size, message.str());
		auto KeDa_sharedPtr = std::make_shared<KeDa>();
		auto JieMai_sharedPtr = std::make_shared<JieMai>();
		auto shared_this = shared_from_this();

		KeDa_sharedPtr->initKeda(cm_.keda_MaxNumConnect);
		KeDa::cb_ = [dbWriter_ptr, KeDa_sharedPtr, shared_this, this](TItsMessage *ptMsg)
		{
			try
			{
				//std::unique_lock<std::mutex> locker(KeDa_sharedPtr->mtx_);
				auto keda_camera_dwHandle = KeDa_sharedPtr->keda_camera_dwHandle_;
				//locker.unlock();
				KeDaCameraIpPort kdcip("", 0);
				for (auto iter = keda_camera_dwHandle.begin(); iter != keda_camera_dwHandle.end(); ++iter)
				{
					if (iter->second == ptMsg->dwHandle)
					{
						kdcip = iter->first;
						break;
					}
				}
				if (kdcip.ip == "" && kdcip.port == 0) //find nothing,means the connection(EV_PEER_DISCONNECT_NTY) is closed normally
					return;

				switch (ptMsg->wMsgType)
				{
				case EV_REGISTER_ACK: //login ok
				{
					VIDEODETECTSERVER_INFO("login camera(%s:%d) ok", kdcip.ip.c_str(), kdcip.port);
				}
				break;
				case EV_IPC_FLOWSTAT_NTY: //FlowStatInfoNty
				{
					CameraIpPort cip("", 0);
					cip.ip = kdcip.ip;
					cip.port = kdcip.port;
					VIDEODETECTSERVER_DEBUG("get data from camera(%s:%d)", cip.ip.c_str(), cip.port);
					std::string camera_number;
					std::vector<std::string> lane_number;
					std::unique_lock<std::mutex> locker_id(number_mtx_);
					// executing in this thread ,and executing in notifyCameraChangedThread at the same time.
					//Remove camera then delete the content in camera_number_(camera_lane_number_) about this camera before get its content in this thread, the result is getting nothing					
					auto iter = camera_number_.find(cip);
					if (iter == camera_number_.end())
						return;
					else
						camera_number = iter->second;
					auto iter1 = camera_lane_number_.find(cip);
					if (iter1 == camera_lane_number_.end())
						return;
					else
						lane_number = iter1->second;
					locker_id.unlock();

					auto lane_number_size = lane_number.size();
					auto vfsin = reinterpret_cast<TIPC_VehFlowStatInfoNty*>(ptMsg->szMsgBody);
					auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
					std::ostringstream temp;
					temp << (std::localtime(&t)->tm_year + 1900) << "-" << (std::localtime(&t)->tm_mon + 1) << "-" << std::localtime(&t)->tm_mday
						<< " " << std::localtime(&t)->tm_hour << ":" << std::localtime(&t)->tm_min << ":" << std::localtime(&t)->tm_sec;
					auto time_str = temp.str();
					if (vfsin->dwLaneId == 0) //all lane has data
					{
						for (int i = 1; i < ROAD_NUM_MAX; i++)
						{
							temp.clear();
							temp.str("");
							temp << "insert into " << cm_.videoDetectDataTable_name << "("
								<< CAMERA_NUMBER << "," << CAMERA_RAW_LANE_NUMBER << "," <<
								CAMERA_CAR_FLOW << "," << CAMERA_UPLOAD_TIME << "," <<
								CAMERA_LANE_NUMBER << ")"
								<< " values (" << "'" << camera_number << "'"
								<< "," << "'" << i << "'"
								<< "," << "'" << vfsin->atFlowStat[i].dwStatCnts << "'"
								<< "," << "to_date('" << time_str << "','YYYY-MM-DD HH24:MI:SS')";
							if (i <= static_cast<int>(lane_number_size))
								temp << "," << "'" << lane_number[i - 1] << "'" << ")";
							else
								temp << "," << "'" << "" << "'" << ")";
							dbWriter_ptr->addSql(temp.str());

							auto sql_str = temp.str();
							auto pos = sql_str.find(cm_.videoDetectDataTable_name);
							auto sql_str_front = sql_str.substr(0, pos);
							auto sql_str_back = sql_str.substr(pos + cm_.videoDetectDataTable_name.size());
							auto sql_history = sql_str_front.append(cm_.videoDetectDataTable_name_history).append(sql_str_back);
							dbWriter_ptr->addSql(sql_history);
						}
					}
					else
					{
						temp.clear();
						temp.str("");
						temp << "insert into " << cm_.videoDetectDataTable_name << "("
							<< CAMERA_NUMBER << "," << CAMERA_RAW_LANE_NUMBER << "," <<
							CAMERA_CAR_FLOW << "," << CAMERA_UPLOAD_TIME << "," <<
							CAMERA_LANE_NUMBER << ")"
							<< " values (" << "'" << camera_number << "'"
							<< "," << "'" << vfsin->dwLaneId << "'"
							<< "," << "'" << vfsin->atFlowStat[vfsin->dwLaneId].dwStatCnts << "'"
							<< "," << "to_date('" << time_str << "','YYYY-MM-DD HH24:MI:SS')";
						if (vfsin->dwLaneId <= static_cast<int>(lane_number_size))
							temp << "," << "'" << lane_number[vfsin->dwLaneId - 1] << "'" << ")";
						else
							temp << "," << "'" << "" << "'" << ")";
						dbWriter_ptr->addSql(temp.str());

						auto sql_str = temp.str();
						auto pos = sql_str.find(cm_.videoDetectDataTable_name);
						auto sql_str_front = sql_str.substr(0, pos);
						auto sql_str_back = sql_str.substr(pos + cm_.videoDetectDataTable_name.size());
						auto sql_history = sql_str_front.append(cm_.videoDetectDataTable_name_history).append(sql_str_back);
						dbWriter_ptr->addSql(sql_history);
					}
				}
				break;
				case EV_DEV_DISCONNECT_NTY:
				case EV_COMM_PEER_DISCONNECT_NTY:
				case EV_PEER_DISCONNECT_NTY:
				{
					auto keda_userName = cm_.keda_userName;
					auto keda_password = cm_.keda_password;
					auto dwHandle = ptMsg->dwHandle;
					VIDEODETECTSERVER_DEBUG("disconnect with camera(%s:%d)", kdcip.ip.c_str(), kdcip.port);
					std::thread thread([KeDa_sharedPtr, kdcip, keda_userName, keda_password, dwHandle]()
					{
						VIDEODETECTSERVER_INFO("disconnect with camera,thread(%d) begin to relogin(%s:%d)", std::this_thread::get_id(), kdcip.ip.c_str(), kdcip.port);
						auto ret = KeDa_sharedPtr->disconnect(dwHandle);
						if (ret == -1)
							VIDEODETECTSERVER_WARN("disconnect(dwHandle:%d) camera(%s:%d) failed in relogin thread(%d)", dwHandle, kdcip.ip.c_str(), kdcip.port, std::this_thread::get_id());
						while (true)
						{
							std::unique_lock<std::mutex> locker(KeDa_sharedPtr->mtx_);
							auto need_connect = KeDa_sharedPtr->keda_camera_need_connect_[kdcip];
							locker.unlock();
							if (need_connect)
							{
								KOSA_HANDLE dHandle = 0;
								ret = KeDa_sharedPtr->connectAndPostLoginMessage(kdcip.ip, kdcip.port, keda_userName, keda_password, dHandle);
								if (ret == 0)
								{
									std::unique_lock<std::mutex> locker_dwHandle(KeDa_sharedPtr->mtx_);
									KeDa_sharedPtr->keda_camera_dwHandle_[kdcip] = dHandle;
									locker_dwHandle.unlock();
									VIDEODETECTSERVER_INFO("connectAndPostLoginMessage(%s:%d) function ok,thread(%d) exit", kdcip.ip.c_str(), kdcip.port, std::this_thread::get_id());
									break;
								}
								else //connect failed										
									std::this_thread::sleep_for(std::chrono::milliseconds(CONNECT_RATE));
							}
							else
							{
								VIDEODETECTSERVER_INFO("camera(%s:%d) do not need connecting,thread(%d) exit", kdcip.ip.c_str(), kdcip.port, std::this_thread::get_id());
								break;
							}
						}
					});
					thread.detach();
				}
				break;
				default:
					break;
				}
			}
			catch (const std::exception& e)
			{
				VIDEODETECTSERVER_ERROR("%s in kedaCallBack", e.what());
			}
			catch (...)
			{
				VIDEODETECTSERVER_ERROR("unknown exception in kedaCallBack");
			}
		};

		JieMai_sharedPtr->initJiemai();
		JieMai::cb_ = [dbWriter_ptr, shared_this, this](OS_INT32 lAlarmType, NET_DEV_ALARMER* /*pAlarmer*/,
			char *pAlarmInfo, OS_UINT32 /*dwBufLen*/, OS_VOIDPTR pUser)
		{
			try
			{
				if (lAlarmType == UPLOAD_TRAFFIC_FLOW)
				{
					auto ip_port_ptr = static_cast<JieMaiCameraIpPort*>(pUser);
					CameraIpPort cip("", 0);
					cip.ip = ip_port_ptr->ip;
					cip.port = ip_port_ptr->port;
					VIDEODETECTSERVER_DEBUG("get data from camera(%s:%d)", cip.ip.c_str(), cip.port);
					std::string camera_number;
					std::vector<std::string> lane_number;
					std::unique_lock<std::mutex> locker_id(number_mtx_);
					// executing in this thread ,and executing in notifyCameraChangedThread at the same time.
					//Remove camera then delete the content in camera_number_(camera_lane_number_) about this camera before get its content in this thread, the result is getting nothing					
					auto iter = camera_number_.find(cip);
					if (iter == camera_number_.end())
						return;
					else
						camera_number = iter->second;
					auto iter1 = camera_lane_number_.find(cip);
					if (iter1 == camera_lane_number_.end())
						return;
					else
						lane_number = iter1->second;
					locker_id.unlock();

					auto lane_number_size = lane_number.size();
					auto AlarmIn = reinterpret_cast<NET_DEV_VEHICLE_FLOW_ALARM*>(pAlarmInfo);
					auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
					std::ostringstream temp;
					temp << (std::localtime(&t)->tm_year + 1900) << "-" << (std::localtime(&t)->tm_mon + 1) << "-" << std::localtime(&t)->tm_mday
						<< " " << std::localtime(&t)->tm_hour << ":" << std::localtime(&t)->tm_min << ":" << std::localtime(&t)->tm_sec;
					auto time_str = temp.str();
					for (int i = 0; i < AlarmIn->s_ValidLaneNum; ++i)
					{
						temp.clear();
						temp.str("");
						temp << "insert into " << cm_.videoDetectDataTable_name << "("
							<< CAMERA_NUMBER << "," << CAMERA_RAW_LANE_NUMBER << "," <<
							CAMERA_CAR_FLOW << "," << CAMERA_UPLOAD_TIME << "," <<
							CAMERA_LANE_NUMBER << ")"
							<< " values (" << "'" << camera_number << "'"
							<< "," << "'" << i + 1 << "'"
							<< "," << "'" << AlarmIn->s_FlowStatInfo[i].s_VehicleTotalCount << "'"
							<< "," << "to_date('" << time_str << "','YYYY-MM-DD HH24:MI:SS')";
						if ((i + 1) <= static_cast<int>(lane_number_size))
							temp << "," << "'" << lane_number[i + 1 - 1] << "'" << ")";
						else
							temp << "," << "'" << "" << "'" << ")";
						dbWriter_ptr->addSql(temp.str());

						auto sql_str = temp.str();
						auto pos = sql_str.find(cm_.videoDetectDataTable_name);
						auto sql_str_front = sql_str.substr(0, pos);
						auto sql_str_back = sql_str.substr(pos + cm_.videoDetectDataTable_name.size());
						auto sql_history = sql_str_front.append(cm_.videoDetectDataTable_name_history).append(sql_str_back);
						dbWriter_ptr->addSql(sql_history);
					}
				}
			}
			catch (const std::exception& e)
			{
				VIDEODETECTSERVER_ERROR("%s in jiemaiCallBack", e.what());
			}
			catch (...)
			{
				VIDEODETECTSERVER_ERROR("unknown exception in jiemaiCallBack");
			}
		};
		JieMai::exception_cb_ = [JieMai_sharedPtr, shared_this, this](OS_UINT32 dwType, OS_INT32 lUserID, OS_UINT32 /*lHandle*/, OS_VOIDPTR /*pUser*/)
		{
			try
			{
				if (dwType == EXCP_HEARTBEAT_FAIL)
				{
					auto jiemai_userName = cm_.jiemai_userName;
					auto jiemai_password = cm_.jiemai_password;
					auto jiemai_cameraId_alarmHandle = JieMai_sharedPtr->jiemai_userId_alarmHandle_;
					JieMaiCameraIpPort jmcip("", 0);
					OS_UINT32 handle = 0;
					for (auto iter = jiemai_cameraId_alarmHandle.begin(); iter != jiemai_cameraId_alarmHandle.end(); ++iter)
					{
						if (iter->second.user_id == lUserID)
						{
							jmcip = iter->first;
							handle = iter->second.alarm_handle;
							break;
						}
					}
					VIDEODETECTSERVER_DEBUG("disconnect with camera(%s:%d)", jmcip.ip.c_str(), jmcip.port);
					std::thread thread([JieMai_sharedPtr, lUserID, jiemai_userName, jiemai_password, jmcip, handle]()
					{
						VIDEODETECTSERVER_INFO("disconnect with camera,thread(%d) begin to relogin(%s:%d)", std::this_thread::get_id(), jmcip.ip.c_str(), jmcip.port);
						auto ret = JieMai_sharedPtr->closeAlarmChan(handle);
						if (ret == -1)
							VIDEODETECTSERVER_WARN("closeAlarmChan(handle:%d) camera(%s:%d) failed in relogin thread(%d)", handle, jmcip.ip.c_str(), jmcip.port, std::this_thread::get_id());
						JieMai_sharedPtr->logout(lUserID);
						if (ret == -1)
							VIDEODETECTSERVER_WARN("logout(user_id:%d) camera(%s:%d) failed in relogin thread(%d)", lUserID, jmcip.ip.c_str(), jmcip.port, std::this_thread::get_id());
						while (true)
						{
							std::unique_lock<std::mutex> locker(JieMai_sharedPtr->mtx_);
							auto need_connect = JieMai_sharedPtr->jiemai_camera_need_connect_[jmcip];
							locker.unlock();
							if (need_connect)
							{
								int user_id = 0;
								int alarm_handle = 0;
								ret = JieMai_sharedPtr->login(jmcip.ip, jmcip.port, jiemai_userName, jiemai_password, user_id, alarm_handle);
								if (ret == 0)
								{
									std::unique_lock<std::mutex> locker_alarmHandle(JieMai_sharedPtr->mtx_);
									JieMai_sharedPtr->jiemai_userId_alarmHandle_[jmcip] = { user_id ,alarm_handle };
									locker_alarmHandle.unlock();
									VIDEODETECTSERVER_INFO("Login camera(%s:%d) ok,thread(%d) exit", jmcip.ip.c_str(), jmcip.port, std::this_thread::get_id());
									break;
								}
								else //connect failed										
									std::this_thread::sleep_for(std::chrono::milliseconds(CONNECT_RATE));
							}
							else
							{
								VIDEODETECTSERVER_INFO("camera(%s:%d) do not need connecting ,thread(%d) exit", jmcip.ip.c_str(), jmcip.port, std::this_thread::get_id());
								break;
							}
						}
					});
					thread.detach();
					//threadPool_ptr->addTask([JieMai_sharedPtr, jmcip, shared_this, this]()->int
					//{
					//	int camera_id = 0;
					//	int alarm_handle = 0;
					//	auto ret = JieMai_sharedPtr->login(jmcip.ip, jmcip.port, cm_.jiemai_userName, cm_.jiemai_password, camera_id, alarm_handle);
					//	if (ret == 0)
					//	{
					//		std::unique_lock<std::mutex> locker(JieMai_sharedPtr->mtx_);
					//		JieMai_sharedPtr->jiemai_userId_alarmHandle_[jmcip] = { camera_id ,alarm_handle };
					//		locker.unlock();
					//		VIDEODETECTSERVER_INFO("Login camera(%s:%d) ok", jmcip.ip.c_str(), jmcip.port);
					//		return WwFoundation::TASK_OK;
					//	}
					//	else //connect failed	
					//	{
					//		std::this_thread::sleep_for(std::chrono::milliseconds(CONNECT_RATE));
					//		return WwFoundation::TASK_RETRY;
					//	}
					//});
				}
			}
			catch (const std::exception& e)
			{
				VIDEODETECTSERVER_ERROR("%s in jiemaiExceptionCallBack", e.what());
			}
			catch (...)
			{
				VIDEODETECTSERVER_ERROR("unknown exception in jiemaiExceptionCallBack");
			}
		};

		bool need_rlogon = true;
		while (run_)
		{
			std::vector<CameraAttribute> camera_attribute;
			try
			{
				if (need_rlogon)
				{
					db_ptr->rlogon(message.str().c_str()/*"wangwei/111111@192.168.1.192:1521/abcdef"*/);
					need_rlogon = false;
				}
				getCameraAttributeFromDB(camera_attribute, *db_ptr);
			}
			catch (otl_exception& p)// intercept OTL exceptions. ex:ORA-12543: TNS:destination host unreachable
			{
				VIDEODETECTSERVER_ERROR("%s", p.msg);//error message
				VIDEODETECTSERVER_ERROR("%s", p.stm_text);//SQL that caused the error
				VIDEODETECTSERVER_ERROR("%s", p.var_info);//the variable that caused the error
				if (db_ptr->connected == 1)
					db_ptr->logoff();
				std::this_thread::sleep_for(std::chrono::milliseconds(retry_rate));
				need_rlogon = true;
				continue;
			}
			std::vector<CameraAttribute> camera_add;
			std::vector<CameraAttribute> camera_decrease;
			auto ret = isCameraChange(camera_attribute, camera_add, camera_decrease);
			if (ret) //camera changed
			{
				if (!camera_add.empty())
				{
					for (auto iter = camera_add.begin(); iter != camera_add.end(); ++iter)
					{
						if (iter->camera_vender == KEDA) //keda camera
						{
							KeDaCameraIpPort kdcip(iter->ip, iter->port);
							std::unique_lock<std::mutex> locker1(KeDa_sharedPtr->mtx_);
							KeDa_sharedPtr->keda_camera_need_connect_[kdcip] = true;
							KeDa_sharedPtr->keda_camera_dwHandle_[kdcip] = 0;
							locker1.unlock();
							VIDEODETECTSERVER_INFO("Add camera(%s:%d-%s)", iter->ip.c_str(), iter->port, iter->camera_vender.c_str());
							//threadPool_ptr->addTask([KeDa_sharedPtr, kdcip, shared_this, this]()->int
							//{
							//	KOSA_HANDLE dHandle = 0;
							//	auto ret = KeDa_sharedPtr->connectAndLogin(kdcip.ip, kdcip.port, cm_.keda_userName, cm_.keda_password, dHandle);
							//	if (ret == 0)
							//	{
							//		std::unique_lock<std::mutex> locker(KeDa_sharedPtr->mtx_);
							//		KeDa_sharedPtr->keda_camera_dwHandle_[kdcip] = dHandle;
							//		locker.unlock();
							//		return WwFoundation::TASK_OK;
							//	}
							//	else //connect failed	
							//	{
							//		std::this_thread::sleep_for(std::chrono::milliseconds(CONNECT_RATE));
							//		return WwFoundation::TASK_RETRY;
							//	}
							//});
							auto keda_userName = cm_.keda_userName;
							auto keda_password = cm_.keda_password;
							auto sub_type = iter->camera_vender;
							std::thread thread([KeDa_sharedPtr, kdcip, keda_userName, keda_password, sub_type]()
							{
								VIDEODETECTSERVER_INFO("thread(%d) begin to login(%s:%d-%s)", std::this_thread::get_id(), kdcip.ip.c_str(), kdcip.port, sub_type.c_str());
								while (true)
								{
									std::unique_lock<std::mutex> locker(KeDa_sharedPtr->mtx_);
									auto need_connect = KeDa_sharedPtr->keda_camera_need_connect_[kdcip];
									locker.unlock();
									if (need_connect)
									{
										KOSA_HANDLE dHandle = 0;
										auto ret = KeDa_sharedPtr->connectAndPostLoginMessage(kdcip.ip, kdcip.port, keda_userName, keda_password, dHandle);
										if (ret == 0)
										{
											std::unique_lock<std::mutex> locker_dwHandle(KeDa_sharedPtr->mtx_);
											KeDa_sharedPtr->keda_camera_dwHandle_[kdcip] = dHandle;
											locker_dwHandle.unlock();
											VIDEODETECTSERVER_INFO("connectAndPostLoginMessage(%s:%d-%s) function ok,thread(%d) exit", kdcip.ip.c_str(), kdcip.port, sub_type.c_str(), std::this_thread::get_id());
											break;
										}
										else //connect failed										
											std::this_thread::sleep_for(std::chrono::milliseconds(CONNECT_RATE));
									}
									else
									{
										VIDEODETECTSERVER_INFO("camera(%s:%d-%s) do not need connecting,thread(%d) exit", kdcip.ip.c_str(), kdcip.port, sub_type.c_str(), std::this_thread::get_id());
										break;
									}
								}
							});
							thread.detach();
						}
						else if (iter->camera_vender == JIEMAI)  //jiemai camera
						{
							JieMaiCameraIpPort jmcip(iter->ip, iter->port);
							std::unique_lock<std::mutex> locker1(JieMai_sharedPtr->mtx_);
							JieMai_sharedPtr->jiemai_camera_need_connect_[jmcip] = true;
							JieMai_sharedPtr->jiemai_userId_alarmHandle_[jmcip] = { 0,0 };
							locker1.unlock();
							VIDEODETECTSERVER_INFO("Add camera(%s:%d-%s)", iter->ip.c_str(), iter->port, iter->camera_vender.c_str());
							//threadPool_ptr->addTask([JieMai_sharedPtr, jmcip, shared_this, this]()->int
							//{
							//	int camera_id = 0;
							//	int alarm_handle = 0;
							//	auto ret = JieMai_sharedPtr->login(jmcip.ip, jmcip.port, cm_.jiemai_userName, cm_.jiemai_password, camera_id, alarm_handle);
							//	if (ret == 0)
							//	{
							//		std::unique_lock<std::mutex> locker(JieMai_sharedPtr->mtx_);
							//		JieMai_sharedPtr->jiemai_cameraId_alarmHandle_[jmcip] = { camera_id ,alarm_handle };
							//		locker.unlock();
							//		VIDEODETECTSERVER_INFO("Login camera(%s:%d) ok", jmcip.ip.c_str(), jmcip.port);
							//		return WwFoundation::TASK_OK;
							//	}
							//	else //connect failed	
							//	{
							//		std::this_thread::sleep_for(std::chrono::milliseconds(CONNECT_RATE));
							//		return WwFoundation::TASK_RETRY;
							//	}
							//});
							auto jiemai_userName = cm_.jiemai_userName;
							auto jiemai_password = cm_.jiemai_password;
							auto sub_type = iter->camera_vender;
							std::thread thread([JieMai_sharedPtr, jmcip, jiemai_userName, jiemai_password, sub_type]()
							{
								VIDEODETECTSERVER_INFO("thread(%d) begin to login(%s:%d-%s)", std::this_thread::get_id(), jmcip.ip.c_str(), jmcip.port, sub_type.c_str());
								while (true)
								{
									std::unique_lock<std::mutex> locker(JieMai_sharedPtr->mtx_);
									auto need_connect = JieMai_sharedPtr->jiemai_camera_need_connect_[jmcip];
									locker.unlock();
									if (need_connect)
									{
										int user_id = 0;
										int alarm_handle = 0;
										auto ret = JieMai_sharedPtr->login(jmcip.ip, jmcip.port, jiemai_userName, jiemai_password, user_id, alarm_handle);
										if (ret == 0)
										{
											std::unique_lock<std::mutex> locker_alarmHandle(JieMai_sharedPtr->mtx_);
											JieMai_sharedPtr->jiemai_userId_alarmHandle_[jmcip] = { user_id ,alarm_handle };
											locker_alarmHandle.unlock();
											VIDEODETECTSERVER_INFO("Login camera(%s:%d-%s) ok,thread(%d) exit", jmcip.ip.c_str(), jmcip.port, sub_type.c_str(), std::this_thread::get_id());
											break;
										}
										else //connect failed										
											std::this_thread::sleep_for(std::chrono::milliseconds(CONNECT_RATE));
									}
									else
									{
										VIDEODETECTSERVER_INFO("camera(%s:%d-%s) do not need connecting ,thread(%d) exit", jmcip.ip.c_str(), jmcip.port, sub_type.c_str(), std::this_thread::get_id());
										break;
									}
								}
							});
							thread.detach();
						}
					}
				}
				if (!camera_decrease.empty())
				{
					for (auto iter = camera_decrease.begin(); iter != camera_decrease.end(); ++iter)
					{
						if (iter->camera_vender == KEDA) //keda camera
						{
							//do not remove the camera,just for next using.ip and port(KeDaCameraIpPort) for next camera 
							//extremely be same in keda_camera_need_connect_(keda_camera_dwHandle_)
							KeDaCameraIpPort kdcip(iter->ip, iter->port);
							KeDa_sharedPtr->keda_camera_need_connect_[kdcip] = false;
							auto dwHandle = KeDa_sharedPtr->keda_camera_dwHandle_[kdcip];
							if (dwHandle)
							{
								KeDa_sharedPtr->keda_camera_dwHandle_[kdcip] = 0;
								KeDa_sharedPtr->disconnect(dwHandle);
							}
							VIDEODETECTSERVER_INFO("Reduce camera(%s:%d-%s)", iter->ip.c_str(), iter->port, iter->camera_vender.c_str());
						}
						else if (iter->camera_vender == JIEMAI)  //jiemai camera
						{
							//do not remove the camera,just for next using.ip and port(JieMaiCameraIpPort) for next camera 
							//extremely be same in jiemai_camera_need_connect_(jiemai_cameraId_alarmHandle_)
							JieMaiCameraIpPort jmcip(iter->ip, iter->port);
							JieMai_sharedPtr->jiemai_camera_need_connect_[jmcip] = false;
							auto cameraId_alarmHandle = JieMai_sharedPtr->jiemai_userId_alarmHandle_[jmcip];
							JieMai_sharedPtr->jiemai_userId_alarmHandle_[jmcip] = { 0,0 };
							if (cameraId_alarmHandle.alarm_handle)
								JieMai_sharedPtr->closeAlarmChan(cameraId_alarmHandle.alarm_handle);
							if (cameraId_alarmHandle.user_id)
								JieMai_sharedPtr->logout(cameraId_alarmHandle.user_id);
							VIDEODETECTSERVER_INFO("Reduce camera(%s:%d-%s)", iter->ip.c_str(), iter->port, iter->camera_vender.c_str());
						}
					}
				}
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(GET_CAMERA_ATTRIBUTE_RATE)); // select from db each 60s
		}
		KeDa_sharedPtr->keda_camera_need_connect_.clear();
		JieMai_sharedPtr->jiemai_camera_need_connect_.clear();
		dbWriter_ptr->stop();
		KeDa_sharedPtr->stopKeda();
		JieMai_sharedPtr->stopJiemai();
	}
	catch (const std::exception& e)
	{
		VIDEODETECTSERVER_ERROR("CameraManager::notifyCameraChangedThread exit with %s", e.what());
	}
	catch (...)
	{
		VIDEODETECTSERVER_ERROR("CameraManager::notifyCameraChangedThread exit with unknown exception");
	}
}
