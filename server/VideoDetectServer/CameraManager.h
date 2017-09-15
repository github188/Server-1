#pragma once
#include <memory>
#include <mutex>
#include <condition_variable>
#include <string>
#include <vector>
#include <queue>
#include <unordered_map>
#include <thread>
class otl_connect;
namespace ITS
{
	const std::string EXCISION = " ";
	const std::string INI_CONFIGURE_DIR = "VideoDetectServer.ini";

	const std::string KEDA = "1";
	const std::string JIEMAI = "2";
	const std::string VIDEO_DETECT = "58";
	const std::string DEVICE_STATUS_OK = "101";

	const std::string ITS_DEVICE_TYPE = "type";  //because  DEVICE_TYPE is defined in the fucking win
	const std::string DEVICE_IP = "ip";
	const std::string DEVICE_PORT = "port";
	const std::string DEVICE_ID = "u_number";
	const std::string DEVICE_SUB_TYPE = "sub_type";
	const std::string DEVICE_UPLOAD_TIME = "upload_time";
	const std::string DEVICE_STATUS_NORMAL = "asset_status";
	const std::string DEVICE_LANE_NUMBER = "lane_number";

	const std::string CAMERA_NUMBER = "camera_number";
	const std::string CAMERA_LANE_NUMBER = "lane_number";
	const std::string CAMERA_CAR_FLOW = "car_flow";
	const std::string CAMERA_UPLOAD_TIME = "upload_time";
	const std::string CAMERA_RAW_LANE_NUMBER = "raw_lane_number";

	const int GET_CAMERA_ATTRIBUTE_RATE = 60000;//ms
	const int CONNECT_RATE = 30000;//ms
	const int DB_WRITER_THREAD_NUMBER = 1;
	const int DB_WRITER_MAX_SQL_COUNT = 2000000; // can store nearly 1G sql in memory ,if each sql is less than 512 byte

	enum class CameraChangeType
	{
		CameraAdd,
		CameraReduce
	};

	typedef struct cameraattribute
	{
		uint16_t port;
		std::string device_id;
		std::string camera_vender;
		std::string ip;
		std::vector<std::string> lane_number;
		bool operator ==(const cameraattribute& ca) const
		{
			return ((port == ca.port) && (device_id == ca.device_id)
				&& (camera_vender == ca.camera_vender) && (ip == ca.ip) && lane_number == ca.lane_number);
		}

	}CameraAttribute;

	typedef struct configmessage
	{
		int thread_num;
		int keda_MaxNumConnect;
		int sql_commit_size;
		std::string db_UserName;
		std::string db_ip;
		std::string db_port;
		std::string db_name;
		std::string db_password;
		std::string deviceTable_name;
		std::string videoDetectDataTable_name;
		std::string keda_userName;
		std::string keda_password;
		std::string jiemai_userName;
		std::string jiemai_password;
	}ConfigMessage;

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

	class CameraManager :public std::enable_shared_from_this<CameraManager>
	{
	private:
		bool run_;
		std::mutex mtx_;
		ConfigMessage cm_;
		std::thread notify_camera_changed_thread_;
		std::thread deal_camera_thread_;
		std::condition_variable has_notify_;
		std::vector<CameraAttribute> camera_attribute_; //do not change frequently,so put into object rather than shared_ptr, just for comparing conveniently
		std::queue<std::pair<CameraChangeType, std::vector<CameraAttribute>>> camera_data_;
		std::mutex number_mtx_;
		std::unordered_map<CameraIpPort, std::string, CameraIpPortHash> camera_number_;
		std::unordered_map<CameraIpPort, std::vector<std::string>, CameraIpPortHash> camera_lane_number_;
	
	public:
		CameraManager() :run_(true) {}
		void init();
		void freeAllResource() 
		{
			run_ = false;
			has_notify_.notify_all();
			if(notify_camera_changed_thread_.joinable())
				notify_camera_changed_thread_.join();
			if (deal_camera_thread_.joinable())
				deal_camera_thread_.join();
		}
	private:
		void getMessageFromIniConfigure(const std::string& dir);
		void getCameraAttributeFromDB(std::vector<CameraAttribute>& camera_attribute, otl_connect & db);
		bool isCameraChange(std::vector<CameraAttribute>& camera_attribute, std::vector<CameraAttribute>& camera_add,
			std::vector<CameraAttribute>& camera_decrease);
		void notifyCameraChangedThread() noexcept;
		void dealCameraThread() noexcept;
	};
}


