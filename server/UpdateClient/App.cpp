#include <algorithm>
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
	std::for_each(ip_set.begin(), ip_set.end(),
		[&client, &port, &instal_packet_name](auto& pair)
	{
		client.addServerInstallMessage(pair.second, port, instal_packet_name);
	});
	client.run();
	client.stop();
	system("pause"); // for reading message about this update
	return 0;
}