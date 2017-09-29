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
	const std::string password, OS_INT32 & user_id, OS_INT32& alarm_handle)
{
	auto userId = NET_DEV_Login(ip.c_str(), port, user_name.c_str(), password.c_str(), NULL);
	if (userId == -1)
	{
		VIDEODETECTSERVER_ERROR("Login camera(%s:%d) failed:%d", ip.c_str(), port, NET_DEV_GetLastError());
		return -1;
	}
	user_id = userId;

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
	auto AlarmHandle = NET_DEV_SetupAlarmChan(userId, &alarmChan, MsgCallback, share_ptr.get());
	if (AlarmHandle == -1)
		VIDEODETECTSERVER_ERROR("SetupAlarmChan(userId:%d) failed:%d\n", userId, NET_DEV_GetLastError());
	alarm_handle = AlarmHandle;
	return 0;
}

int ITS::JieMai::logout(OS_INT32 user_id)
{
	auto ret = NET_DEV_Logout(user_id);
	if (ret == OS_FALSE)
	{
		VIDEODETECTSERVER_ERROR(" NET_DEV_Logout(user_id:%d) failed:%d", user_id, NET_DEV_GetLastError());
		return -1;
	}	
	return 0;
}

int ITS::JieMai::stopJiemai()
{
	for (auto iter : jiemai_userId_alarmHandle_)
	{
		if (iter.second.alarm_handle)
			closeAlarmChan(iter.second.alarm_handle);
		if (iter.second.user_id)
			logout(iter.second.user_id);	
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
