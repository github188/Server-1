#pragma once
#ifndef SERVER_SERVER_UPDATESERVER_UPDATEINSTALLPACKET_H
#define SERVER_SERVER_UPDATESERVER_UPDATEINSTALLPACKET_H

#include <string>

namespace update
{
	namespace server
	{
		struct UpdateInstallPacket
		{
			void update(const std::string& install_packet_name, const std::string& last_time_components);
		};
	}
}
#endif