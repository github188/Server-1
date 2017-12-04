#include "ParseIni.h"
#include "Client.h"
#include <thread>
int main()
{
	boost::asio::io_service io_service;
	auto client_ptr= std::make_shared<update::Client>(io_service, "192.168.1.199", static_cast<uint16_t>(7000), "update.exe");
	client_ptr->go();
	io_service.run();
	
	return 0;
}