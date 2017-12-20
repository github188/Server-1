#pragma once
#ifndef SERVER_SERVER_UPDATESERVER_SERVER_H
#define SERVER_SERVER_UPDATESERVER_SERVER_H

#include <string>
namespace update
{
	namespace server
	{
		const std::string INI_DIR = "updateServer.ini";

		class Server
		{
		public:
			void run();
		};
	}
}
#endif