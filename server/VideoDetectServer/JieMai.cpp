#include "JieMai.h"
#include <sstream>
#include "LogMacros.h"

ITS::JieMai::CameraMessageCallback ITS::JieMai::cb_; //define
ITS::JieMai::CameraExceptionCallback ITS::JieMai::exception_cb_;//define

void ITS::JieMai::initJiemai()
{
	auto ret = NET_DEV_Init();
	if (ret == OS_FALSE)
	{
		std::ostringstream str;
		str << "NET_DEV_Init failed with error_code(%d): " << NET_DEV_GetLastError();
		throw std::runtime_error(str.str());
	}

	ret = NET_DEV_SetExceptionCallback(ExceptionCallback, nullptr);
	if (ret == OS_FALSE)
		VIDEODETECTSERVER_ERROR("NET_DEV_SetExceptionCallback failed:%d", NET_DEV_GetLastError());

	/*ret = NET_DEV_SetReconnect(true, 3000);
	if (ret == OS_FALSE)
		VIDEODETECTSERVER_ERROR("NET_DEV_SetReconnect failed:%d", NET_DEV_GetLastError());*/
}

int ITS::JieMai::login(const std::string ip, const uint16_t port, const std::string user_name,
	const std::string password, OS_INT32 & camera_id, OS_INT32& alarm_handle)
{
	auto cameraId = NET_DEV_Login(ip.c_str(), port, user_name.c_str(), password.c_str(), NULL);
	if (cameraId == -1)
	{
		VIDEODETECTSERVER_ERROR("Login camera(%s:%d) failed:%d", ip.c_str(), port, NET_DEV_GetLastError());
		return -1;
	}
	camera_id = cameraId;

	std::shared_ptr<JieMaiCameraIpPort> share_ptr;
	for (auto share_ptr_temp : ip_port_for_cb_)
	{
		if (share_ptr_temp->ip == ip && share_ptr_temp->port == port)
		{
			share_ptr = share_ptr_temp;
			break;
		}
	}
	if (share_ptr == nullptr)
	{
		share_ptr = std::make_shared<JieMaiCameraIpPort>(ip, port);
		ip_port_for_cb_.emplace_back(share_ptr);
	}
	
	ALARM_CHAN_ARA alarmChan;
	memset(&alarmChan, 0, sizeof(ALARM_CHAN_ARA));
	auto AlarmHandle = NET_DEV_SetupAlarmChan(camera_id, &alarmChan, MsgCallback, share_ptr.get());
	if (AlarmHandle == -1)
		VIDEODETECTSERVER_ERROR("SetupAlarmChan(camera_id:%d) failed:%d\n", cameraId, NET_DEV_GetLastError());
	alarm_handle = AlarmHandle;
	return 0;
}

int ITS::JieMai::logout(OS_INT32 camera_id)
{
	auto ret = NET_DEV_Logout(camera_id);
	if (ret == OS_FALSE)
	{
		VIDEODETECTSERVER_ERROR(" NET_DEV_Logout(camera_id:%d) failed:%d", camera_id, NET_DEV_GetLastError());
		return -1;
	}	
	return 0;
}

int ITS::JieMai::stopJiemai()
{
	for (auto iter : jiemai_cameraId_alarmHandle_)
	{
		if (iter.second.alarm_handle)
			closeAlarmChan(iter.second.alarm_handle);
		if (iter.second.camera_id)
			logout(iter.second.camera_id);	
	}
	auto ret = NET_DEV_Cleanup();
	if (ret == OS_FALSE)
	{
		VIDEODETECTSERVER_ERROR("NET_DEV_Cleanup failed:%d", NET_DEV_GetLastError());
		return -1;
	}
	return 0;
}

int ITS::JieMai::closeAlarmChan(const OS_INT32 alarm_handle)
{
	auto ret = NET_DEV_CloseAlarmChan(alarm_handle);
	if (ret == OS_FALSE)
	{
		VIDEODETECTSERVER_ERROR("NET_DEV_CloseAlarmChan(alarm_handle:%d) failed:%d", alarm_handle, NET_DEV_GetLastError());
		return -1;
	}
	return 0;
}
