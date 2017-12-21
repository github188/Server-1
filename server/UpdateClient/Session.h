#pragma once
#ifndef SERVER_CLIENT_UPDATECLIENT_SESSION_H
#define SERVER_CLIENT_UPDATECLIENT_SESSION_H
#include <vector>
#include "boost/asio/ip/tcp.hpp"
#include "boost/asio/windows/random_access_handle.hpp"
#include "boost/asio/io_service.hpp"

namespace update
{
	namespace client
	{
		const int HEAD_SIZE = 16;
		const int SEND_SIZE_MAX = 1024;

		class Session :public std::enable_shared_from_this<Session>
		{
		private:
			boost::asio::ip::tcp::socket socket_;
			boost::asio::ip::tcp::endpoint endpoint_;
			std::vector<std::string> update_file_name_vec_;
			std::vector<std::shared_ptr<boost::asio::windows::random_access_handle>> file_vec_;
		public:
			explicit Session(boost::asio::io_service& io_service, const std::string& server_addr,
				const uint16_t port,const std::vector<std::string>& update_file_name_vec)
				:socket_(io_service),
				endpoint_(boost::asio::ip::address::from_string(server_addr), port),
				update_file_name_vec_(std::move(update_file_name_vec)) {}

			void go();
		private:
			void setPacketRemain(std::shared_ptr<char> send_buffer, const char* data_buffer, const uint16_t data_size);
		private:
			Session(const Session&) = delete;
			Session& operator=(const Session&) = delete;
		};
	}
}
#endif