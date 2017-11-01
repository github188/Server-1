#pragma once
#include <string>
#include <functional>
#include <mutex>
#include <unordered_map>
#include <vector>
#include <memory>
#include "net_sdk.h"
#include "CameraAttribute.h"
namespace ITS
{
	typedef struct jiemaiuseridalarmhandle
	{
		int user_id;
		int alarm_handle;
	}JieMaiUserIdAlarmHandle;

	class JieMai
	{
	public:
		typedef std::function<void(OS_INT32 lAlarmType, NET_DEV_ALARMER *pAlarmer, char *pAlarmInfo,
			OS_UINT32 dwBufLen, OS_VOIDPTR pUser)> CameraMessageCallback;
		static CameraMessageCallback cb_; //declare
		
		typedef std::function<void(OS_UINT32 dwType, OS_INT32 lUserID, OS_UINT32 lHandle, OS_VOIDPTR pUser)> CameraExceptionCallback;
		static CameraExceptionCallback exception_cb_;//declare

		std::mutex mtx_;
		std::unordered_map<CameraAttribute, bool, CameraAttributeHash> jiemai_camera_need_connect_;
		std::unordered_map<CameraAttribute, JieMaiUserIdAlarmHandle, CameraAttributeHash> jiemai_userId_alarmHandle_;
	public:
		void initJiemai();
		int login(const std::string ip, const uint16_t port,
			const std::string user_name, const std::string password,
			OS_INT32& user_id, OS_INT32& alarm_handle, void* user_data);
		int logout(OS_INT32 user_id);
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