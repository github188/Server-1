#pragma once
#ifndef SERVER_CLIENT_UPDATECLIENT_H
#define SERVER_CLIENT_UPDATECLIENT_H
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

namespace update { 
	namespace client { 
		class IoServicePool;
		//class Session;
	} 
}

namespace update
{
	namespace client
	{
		typedef struct serveripport
		{
			std::string ip;
			uint16_t port;

			serveripport(const std::string& Ip, uint16_t Port) :ip(Ip), port(Port) {}

			bool operator ==(const serveripport sip) const
			{
				return (ip == sip.ip && port == sip.port);
			}
		} ServerIpPort;

		struct ServerIpPortHash
		{
			std::size_t operator()(const ServerIpPort& k) const
			{
				auto h1 = std::hash<std::string>()(k.ip);
				auto h2 = std::hash<uint16_t>()(k.port);
				return (h2 << 1) ^ (h1 >> 1); // or use boost::hash_combine
			}
		};

		class Client /*:public std::enable_shared_from_this<Client>*/
		{
		private:
			std::unordered_map<ServerIpPort, std::vector<std::string>, ServerIpPortHash> server_install_message_;
			std::shared_ptr<IoServicePool> io_service_pool_ptr_;
		public:
			Client(){}
			void makeSession(const std::string& ip, const uint16_t port,
				const std::vector<std::string>& update_file_name_vec);
			void makeSession(const std::string& ip, const uint16_t port, const std::string& update_file_name_vec);
			void run();
			void stop();
		private:
			Client(const Client& client) = delete;
			Client& operator=(const Client&) = delete;
		private:

		};
	}
}
#endif