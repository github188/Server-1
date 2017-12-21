#include <algorithm>
#include <thread>
#include "ParseIni.h"
#include "Client.h"

int main()
{
	WwFoundation::ParseIni paser("updateClient.ini");
	paser.parse();
	auto instal_packet_name = paser.getValue("InstallPacketNmaeConfig", "name");
	auto port = static_cast<uint16_t>(std::atoi(paser.getValue("PortConfig", "port").c_str()));
	auto ip_set = paser.getSectionValue("IpConfig");

	update::client::Client client;
	client.run();
	std::for_each(ip_set.begin(), ip_set.end(),
		[&client, &port, &instal_packet_name](auto& pair)
	{
		client.makeSession(pair.second, port, instal_packet_name);
	});
	std::this_thread::sleep_for(std::chrono::milliseconds(1000)); //for io_service do real work
	client.stop();
	system("pause"); // for reading message about this update
	return 0;
}