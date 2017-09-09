#pragma once
#include <string>
#include <functional>
#include <mutex>
#include <unordered_map>
#include <vector>
#include <memory>
#include "net_sdk.h"
namespace ITS
{
	typedef struct jiemaicameraipport
	{
		std::string ip;
		uint16_t port;

		jiemaicameraipport(const std::string& Ip, uint16_t Port) :ip(Ip), port(Port) {}

		bool operator ==(const jiemaicameraipport cip) const
		{
			return (ip == cip.ip && port == cip.port);
		}
	} JieMaiCameraIpPort;

	struct JieMaiCameraIpPortHash
	{
		std::size_t operator()(const JieMaiCameraIpPort& k) const
		{
			auto h1 = std::hash<std::string>()(k.ip);
			auto h2 = std::hash<uint16_t>()(k.port);
			return (h2 << 1) ^ (h1 >> 1); // or use boost::hash_combine
		}
	};

	typedef struct jiemaicameraidalarmhandle
	{
		int camera_id;
		int alarm_handle;
	}JieMaiCameraIdAlarmHandle;

	class JieMai
	{
	public:
		typedef std::function<void(OS_INT32 lAlarmType, NET_DEV_ALARMER *pAlarmer, char *pAlarmInfo,
			OS_UINT32 dwBufLen, OS_VOIDPTR pUser)> CameraMessageCallback;
		static CameraMessageCallback cb_; //declare
		
		typedef std::function<void(OS_UINT32 dwType, OS_INT32 lUserID, OS_UINT32 lHandle, OS_VOIDPTR pUser)> CameraExceptionCallback;
		static CameraExceptionCallback exception_cb_;//declare

		std::mutex mtx_;
		std::unordered_map<JieMaiCameraIpPort, bool, JieMaiCameraIpPortHash> jiemai_camera_need_connect_;
		std::unordered_map<JieMaiCameraIpPort, JieMaiCameraIdAlarmHandle, JieMaiCameraIpPortHash> jiemai_cameraId_alarmHandle_;
		std::vector<std::shared_ptr<JieMaiCameraIpPort>> ip_port_for_cb_;
	public:
		void initJiemai();
		int login(const std::string ip, const uint16_t port,
			const std::string user_name, const std::string password,
			OS_INT32& camera_id, OS_INT32& alarm_handle);
		int logout(OS_INT32 camera_id);
		int stopJiemai();
		int closeAlarmChan(const OS_INT32 alarm_handle);
	private:
		static void SDKCALL MsgCallback(OS_INT32 lAlarmType, NET_DEV_ALARMER *pAlarmer,
			char *pAlarmInfo, OS_UINT32 dwBufLen, OS_VOIDPTR pUser)
		{
			cb_(lAlarmType, pAlarmer, pAlarmInfo, dwBufLen, pUser);
		}

		static void SDKCALL ExceptionCallback(OS_UINT32 dwType, OS_INT32 lUserID, OS_UINT32 lHandle, OS_VOIDPTR pUser)
		{
			exception_cb_(dwType, lUserID, lHandle, pUser);
		}
	};
}