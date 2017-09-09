#include"CameraManager.h"
#include "crashDump.h"
#include "LogMacros.h"
int main()
{
	crashDump();
	auto ptr = std::make_shared<ITS::CameraManager>();

	glog_init("VideoDetectServer");
	VIDEODETECTSERVER_INFO( "VideoDetectServer start");
	ptr->init();
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(3000));
	}
	ptr->freeAllResource();
	printf("11");
}