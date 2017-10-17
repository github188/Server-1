#pragma once
#ifndef SERVER_MONITORSERVER_DEALDEVICEDATA_H
#define SERVER_MONITORSERVER_DEALDEVICEDATA_H

#include <unordered_map>
#include <map>
#include <memory>
#include <string>
#include <stdexcept>
#include <exception>
#include <thread>
#include <any>
#include <mutex>

class otl_connect;
namespace WwFoundation { class DbWriter; }
namespace Platform
{
	const int SEND_OID_COUNT = 30;
	const std::string CONFIGURE_DIR = "type_oid.json";
	const std::string INI_CONFIGURE_DIR = "MoniterServer.ini";
	//device data table
	const std::string DEVICE_MODEL = "model";
	const std::string DEVICE_IP = "ip";
	const std::string DEVICE_ID = "u_number";
	const std::string DEVICE_UPLOAD_TIME = "upload_time";
	const std::string DEVICE_CPU_RATE = "cpu_use_rate";
	const std::string DEVICE_CPU_TEMPERATURE = "cpu_temperature";
	const std::string DEVICE_MEMORY_RATE = "mem_use_rate";
	const std::string DEVICE_BANDWIDTH_USE_AVERAGE = "net_use_rate";
	const std::string DEVICE_PORT_BANDWIDTH_MAX = "port_use_maxrate";
	const std::string DEVICE_DISK_RATE = "disk_use_rate";
	const std::string DEVICE_SYSTEM_DESCRIPTION = "system_desc";
	const std::string DEVICE_REMARK = "remark";
	const std::string DEVICE_STATUS = "state";
	//another table(deviceTable)
	const std::string CAMERA = "51";
	const std::string DEVICE_STATUS_OK = "101";
	const std::string DEV_STATUS_NORMAL = "asset_status";
	const std::string DEVICE_WORK_STATUS = "work_status";
	const std::string DEVICE_CODE = "device_code";
	const std::string DEVICE_PORT = "port";
	const std::string ITS_DEVICE_TYPE = "type";  //because  DEVICE_TYPE is defined in the fucking win
	const std::string DEVICE_ONLINE = "1";
	const std::string DEVICE_OFFLINE = "0";
	const std::string CAMERA_ON_VIDEO = "2"; //must be online
	//alarm log table
	const std::string ALARM_TYPE_ONLINE = "1151";
	const std::string ALARM_TYPE_PARAMETER_OK = "1152";
	const std::string ALARM_TYPE_PARAMETER_ABNORMAL = "1153";
	const std::string ALARM_TYPE_OFFLINE = "1154";
	const std::string ALARM_LEVEL_FATAL = "1201";
	const std::string ALARM_LEVEL_SERIOUS = "1202";
	const std::string ALARM_LEVEL_ALARM = "1203";
	const std::string ALARM_LEVEL_ORDINARY = "1204";
	const std::string ALARM_DEAL_STATUS_NO_DEAL = "0";
	const std::string ALARM_DEAL_STATUS_WITH_DEAL = "1";
	const std::string ALARM_TYPE = "alarm_type"; // 1151-device online ,1152-device parameter ok,1153-device parameter abnormal,1154-device offline
	const std::string ALARM_TIME = "alarm_time";
	const std::string ALARM_LEVEL = "alarm_level";//1201-fatal(1154),1202-serious(device parameter become maximum),1203-alarm(device parameter over threshold).1204-ordinary(1151,1152)
	const std::string ALARM_DETAIL = "alarm_detail";
	const std::string ALARM_ADDTIONAL = "alarm_addtional";
	const std::string ALARM_DEAL_STATUS = "alarm_deal_status";// 0- no deal ,1- with deal
	//another db(ivp:platform -camera)
	const std::string CAMERA_STATUS_CODE = "StatusCode";
	const std::string CAMERA_IPV4 = "IPv4";
	const std::string CAMERA_PORT = "Port";
	const std::string CAMERA_OFFLINE = "1";
	const std::string CAMERA_ONLINE = "3";
	
	typedef struct configmessage
	{
		std::string dbUser_name_oracle;
		std::string db_ip_oracle;
		std::string db_port_oracle;
		std::string db_name_oracle;
		std::string db_password_oracle;
		std::string deviceTable_name_oracle;
		std::string device_alarmLogTable_name_oracle;
		std::string deviceDataTable_name_oracle;
		std::string dbUser_name_mysql;
		std::string db_ip_mysql;
		std::string db_port_mysql;
		std::string db_name_mysql;
		std::string db_password_mysql;
		std::string cameraTable_name_mysql;

		std::string snmp_community;
		int load_count;
		int moniter_rate;//ms
	}ConfigMessage;

	class DealDeviceData;
	typedef struct impl
	{
		std::shared_ptr<DealDeviceData> this_ptr;
		std::shared_ptr<std::mutex> mutex_ptr;
	}Impl;

	typedef struct portflow
	{
		uint32_t in;
		uint32_t out;
		uint32_t ifSpeed;
	} PortFlow;

	typedef struct devicecompareattribute
	{
		std::string deviceModel;
		std::string deviceIp;
		std::string deviceId;
		bool operator ==(const devicecompareattribute& da) const
		{
			return ((deviceModel == da.deviceModel) && (deviceIp == da.deviceIp) && (deviceId == da.deviceId));
		}
	} DeviceCompareAttribute;

	typedef struct deviceattribute
	{
		DeviceCompareAttribute dca;
		void* session;
		void* manager_entity;
		void* context;
		void* vbl;
		void* agent_entity;
		void* pdu;
	}DeviceAttribute;

	//struct load_sessionHash
	//{
	//	std::size_t operator()(const std::shared_ptr<std::vector<DeviceAttribute>>& ptr) const
	//	{
	//		//auto h1 = std::hash<std::vector<DeviceAttribute>*>()(ptr.get());
	//		//return (h1 << 1)^ (h1 >> 1); // or use boost::hash_combine
	//	}
	//};
	struct load_session
	{
		struct Hash
		{
			template <typename T>
			std::size_t operator() (const std::shared_ptr<T>& ptr) const
			{
				return ((size_t)ptr.get()) / sizeof(T);
			}
		};
	};

	struct DeviceCompareAttributeHash
	{
		std::size_t operator()(const DeviceCompareAttribute& k) const
		{
			auto h1 = std::hash<std::string>()(k.deviceId);
			auto h2 = std::hash<std::string>()(k.deviceIp);
			auto h3 = std::hash<std::string>()(k.deviceModel);
			return h1 ^ (h2 << 1) ^ (h3 >> 1); // or use boost::hash_combine
		}
	};

	typedef std::unordered_map<std::string, std::unordered_map<std::string, std::string>> ModelOid;
	typedef std::unordered_map<DeviceCompareAttribute, bool, DeviceCompareAttributeHash> IsOidWhole;
	typedef std::unordered_map<DeviceCompareAttribute, std::vector<PortFlow>, DeviceCompareAttributeHash> DevicePortFlow;
	typedef std::unordered_map<DeviceCompareAttribute, std::map<std::string, std::any>, DeviceCompareAttributeHash> DeviceData;

	class Snmp;
	class DealDeviceData sealed :public std::enable_shared_from_this<DealDeviceData>
	{
	private:
		bool run_;
		int device_count_;
		std::shared_ptr<Snmp> snmp_;
		std::thread thread_moniter_;
		std::thread thread_moniter_offline_;
		std::thread thread_monitor_camera_status_;
		std::vector<std::shared_ptr<Impl>> impl_;
		std::shared_ptr<WwFoundation::DbWriter> db_writer_;
		std::vector<DeviceCompareAttribute> device_compare_attribute_;
		std::vector<std::shared_ptr<std::vector<DeviceAttribute>>> load_; //every load has some devices
		std::unordered_map<std::string, std::unordered_map<std::string, std::string>> model_oid_; //every model has a group of oids
		std::unordered_map<DeviceCompareAttribute, bool, DeviceCompareAttributeHash> is_oid_whole_;// since a group of oids too big, must split send request. Makesure get all request,or do not deal this missing device data
		std::unordered_map<DeviceCompareAttribute, int, DeviceCompareAttributeHash> device_online_flag_;// online=1, offline=0
		std::unordered_map<std::shared_ptr<std::vector<DeviceAttribute>>, void*, load_session::Hash> load_session_; //every load has a session which be bound to a system thread
		std::unordered_map<DeviceCompareAttribute, std::vector<PortFlow>, DeviceCompareAttributeHash> device_port_flow_;//one or several port flow, the data be put into struct PortFlow	
		std::unordered_map<DeviceCompareAttribute, std::map<std::string,std::any>, DeviceCompareAttributeHash> device_data_;//the data return by snmp request
		std::unordered_map<std::shared_ptr<std::vector<DeviceAttribute>>, std::shared_ptr<std::mutex>, load_session::Hash > load_mutex_; //every load has a mutex

	public:
		bool moniterThread_exception_exit_;
		ConfigMessage cm_;
	public:
		explicit DealDeviceData();
		~DealDeviceData() {};/*noexcept*/;

		void init();
		void freeAllResource();
		std::shared_ptr<Snmp> getSnmp() const{ return snmp_; }
		ModelOid& getModelOid() { return model_oid_; }
		DevicePortFlow& getPortflow(){ return device_port_flow_; }
		std::vector<DeviceCompareAttribute>& getDeviceCompareAttribute() { return device_compare_attribute_; }
		DeviceData& getDeviceData() { return device_data_; }
		IsOidWhole& getIsOidWhole() { return is_oid_whole_; }
		auto& getDeviceOnlineFlag() { return device_online_flag_; }
		int getDbWriterThreadAliveCount()const;
		auto getDbWriterPtr() const { return db_writer_; }
	private:
		void getMessageFromIniConfigure(const std::string& dir);
		void getMessageFromConfigure(const std::string& dir);
		void getDeviceIpFromDB(std::vector<DeviceCompareAttribute>& device_compare_attribute, otl_connect& db);
		bool isDeviceChange(std::vector<DeviceCompareAttribute>& device_compare_attribute,
			std::vector<DeviceCompareAttribute>& add_device, std::vector<DeviceCompareAttribute>& decrease_device);//device change,return true
		void setDevcieAndLoadBalancing(std::vector<DeviceCompareAttribute>& add, const std::vector<DeviceCompareAttribute>& decrease);
		void makeLoadsession();
		int setSnmpParameter(void* session, void*& manager_entity, void*& context, void*& vbl,
			void*& agent_entity, void*& pdu, const DeviceCompareAttribute& dca);
		void releaseDeviceResource(void* session, void* context, void* vbl,
			void* manager_entity, void* agent_entity, void* pdu);
		void deviceMonitorThread() noexcept;
		void deviceOfflineMonitorThread() noexcept;
		void cameraStatusMonitorThread() noexcept;
	};
}
#endif


