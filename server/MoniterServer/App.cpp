#include "DealDeviceData.h"
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
			if (ptr->moniterThread_exception_exit_ || (ptr->getDbWriterThreadAliveCount() == 0))
				break;
			std::this_thread::sleep_for(std::chrono::milliseconds(3000));
		}
	}
	catch (const std::exception& e)
	{
		MONITERSERVER_ERROR("main:%s", e.what());
	}
	catch (...)
	{
		MONITERSERVER_ERROR("%s", "main: unknown exception!!");
	}
	ptr->freeAllResource();
	return 0;
}