#include "DealDeviceData.h"
#include "glog.h"
#include "LogMacros.h"
#include "crashDump.h"
int main()
{
	crashDump();
	std::shared_ptr<Platform::DealDeviceData> ptr(new Platform::DealDeviceData);
	try
	{
		glog_init("MoniterServer");
		MONITERSERVER_INFO("%s", "MoniterServer start");
		ptr->init();
		while (true)
		{
			if (!ptr->moniter_thread_alive_)
				break;
#if _MSC_VER >=1910
			std::this_thread::sleep_for(std::chrono::milliseconds(3000));
#else
			boost::this_thread::sleep_for(boost::chrono::milliseconds(3000));
#endif
		}
	}
	catch (const std::exception& e)
	{
		MONITERSERVER_ERROR("%s", e.what());
	}
	catch (...)
	{
		MONITERSERVER_ERROR("%s", "unknown exception!!");
	}
	ptr->freeAllResource();
	return 0;
}