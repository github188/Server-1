#pragma once
#ifndef SERVER_MONITORSERVER_DEALDEVICEDATA_H
#define SERVER_MONITORSERVER_DEALDEVICEDATA_H

#include <unordered_map>
#include <map>
#include <memory>
#include <string>
#include <stdexcept>
#include <exception>
#if _MSC_VER >=1800
#include <thread>
#include <any>
#include <mutex>
#else
#include "boost/thread.hpp"
#include "boost/any.hpp"
#include "boost/thread/mutex.hpp"
#endif

class otl_connect;
namespace Platform
{
	
	const int SEND_OID_COUNT = 30;
	const std::string CONFIGURE_DIR = "type_oid.json";
	const std::string INI_CONFIGURE_DIR = "MoniterServer.ini";

	//const int LOAD_COUNT =2;
	//const int MONITOR_RATE = 30000;//ms
	/*const std::string DEVICE_TABLE = "ITS_DEVICE";
	const std::string DEVICE_DATA_TABLE = "ITS_RUN_STATE";
	const std::string DB_MESSAGE = "sde/111111@192.168.1.157:1521/testorcl";*/
	//const std::string DEVICE_TABLE = "DEVICE";
	//const std::string DEVICE_DATA_TABLE = "DEVICEdata";
	//const std::string DB_MESSAGE = "wangwei/111111@192.168.1.192:1521/abcdef"; 

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

	typedef struct configmessage
	{
		std::string dbUser_name;
		std::string db_ip;
		std::string db_port;
		std::string db_name;
		std::string db_password;
		std::string deviceTable_name;
		std::string deviceDataTable_name;
		int load_count;
		int moniter_rate;//ms
	}ConfigMessage;

	class DealDeviceData;
	typedef struct impl
	{
		std::shared_ptr<otl_connect> db;
		std::shared_ptr<DealDeviceData> this_ptr;
#if _MSC_VER >=1910
		std::shared_ptr<std::mutex> mutex_ptr;
#else
		std::shared_ptr<boost::mutex> mutex_ptr;
#endif
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
#if _MSC_VER >=1800
	typedef std::unordered_map<DeviceCompareAttribute, std::map<std::string, std::any>, DeviceCompareAttributeHash> DeviceData;
#else
	typedef std::unordered_map<DeviceCompareAttribute, std::map<std::string,boost::any>, DeviceCompareAttributeHash> DeviceData;
#endif

	class Snmp;
	class DealDeviceData sealed :public std::enable_shared_from_this<DealDeviceData>
	{
	private:
		bool run_;
		int device_count_;
		std::shared_ptr<Snmp> snmp_;
		std::vector<std::shared_ptr<Impl>> impl_;
		std::vector<DeviceCompareAttribute> device_compare_attribute_;
		std::vector<std::shared_ptr<std::vector<DeviceAttribute>>> load_; //every load has some devices
		std::unordered_map<std::string, std::unordered_map<std::string, std::string>> model_oid_; //every model has a group of oids
		std::unordered_map<DeviceCompareAttribute, bool, DeviceCompareAttributeHash> is_oid_whole_;// since a group of oids too big, must split send request. Makesure get all request,or do not deal this missing device data
		std::unordered_map<std::shared_ptr<std::vector<DeviceAttribute>>, void*, load_session::Hash> load_session_; //every load has a session which be bound to a system thread
		std::unordered_map<DeviceCompareAttribute, std::vector<PortFlow>, DeviceCompareAttributeHash> device_port_flow_;//one or several port flow, the data be put into struct PortFlow	
#if _MSC_VER >=1800
		std::thread thread_moniter_;
		std::unordered_map<DeviceCompareAttribute, std::map<std::string,std::any>, DeviceCompareAttributeHash> device_data_;//the data return by snmp request
		std::unordered_map<std::shared_ptr<std::vector<DeviceAttribute>>, std::shared_ptr<std::mutex>, load_session::Hash > load_mutex_; //every load has a mutex
#else
		boost::thread thread_moniter_;
		std::unordered_map<DeviceCompareAttribute, std::map<std::string,boost::any>, DeviceCompareAttributeHash> device_data_;
		std::unordered_map < std::shared_ptr<std::vector<DeviceAttribute>>, std::shared_ptr<boost::mutex>, load_session::Hash > load_mutex_;
#endif
	public:
		bool moniterThread_exception_exit_;
		ConfigMessage cm_;
	public:
		explicit DealDeviceData();
		~DealDeviceData() {};/*noexcept*/;

		void init();
		void freeAllResource();
		std::shared_ptr<Snmp> getSnmp(){ return snmp_; }
		ModelOid& getModelOid(){ return model_oid_; }
		DevicePortFlow& getPortflow(){ return device_port_flow_; }
		std::vector<DeviceCompareAttribute>& getDeviceCompareAttribute(){ return device_compare_attribute_; }
		DeviceData& getDeviceData(){ return device_data_; }
		IsOidWhole& getIsOidWhole(){ return is_oid_whole_; }
	private:
		void getMessageFromIniConfigure(const std::string dir);
		void getMessageFromConfigure(const std::string dir);
		void getDeviceIpFromDB(std::vector<DeviceCompareAttribute>& device_compare_attribute, otl_connect& db);
		bool isDeviceChange(std::vector<DeviceCompareAttribute>& device_compare_attribute,
			std::vector<DeviceCompareAttribute>& add_device, std::vector<DeviceCompareAttribute>& decrease_device);//device change,return true
		void setDevcieAndLoadBalancing(std::vector<DeviceCompareAttribute>& add, std::vector<DeviceCompareAttribute>& decrease);
		void makeLoadsession();
		int setSnmpParameter(void* session, void*& manager_entity, void*& context, void*& vbl,
			void*& agent_entity, void*& pdu, const DeviceCompareAttribute& dca);
		void releaseDeviceResource(void* session, void* context, void* vbl,
			void* manager_entity, void* agent_entity, void* pdu);
		void deviceMonitorThread() /*noexcept*/;
	};
}
#endif


