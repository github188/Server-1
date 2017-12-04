#pragma once
#ifndef SERVER_CLIENT_UPDATECLIENT_H
#define SERVER_CLIENT_UPDATECLIENT_H
#include <cstdint>
#include <string>
#include "boost/asio/io_service.hpp"
#include "boost/asio/ip/tcp.hpp"
#include "boost/asio/spawn.hpp"
#include "boost/asio/write.hpp"
#include "boost/asio/read.hpp"
#include "boost/asio/windows/random_access_handle.hpp"
namespace update
{
	const int head_size = 16;
	const int send_size_max = 1024;
	class Client :public std::enable_shared_from_this<Client>
	{
	private:
		boost::asio::ip::tcp::socket socket_;
		boost::asio::ip::tcp::endpoint endpoint_;
		boost::asio::windows::random_access_handle file_;
		std::string install_packet_name_;
		bool run_;
	public:
		explicit Client(boost::asio::io_service& io_service, const std::string& server_addr,
			const uint16_t port, const std::string& install_packet_name)
			:socket_(io_service),
			endpoint_(boost::asio::ip::address::from_string(server_addr), port),
			file_(io_service),
			install_packet_name_(install_packet_name),
			run_(true)
		{}

		void go()
		{
			auto self(shared_from_this());
			boost::asio::spawn(socket_.get_io_service(), [this, self](boost::asio::yield_context yield)
			{
				boost::system::error_code ec;
				file_.assign(CreateFile(install_packet_name_.c_str(), GENERIC_READ, 0, 0,
					OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 0), ec);
				try
				{
					socket_.async_connect(endpoint_, yield);
					auto buffer_size = head_size + install_packet_name_.length();
					auto buffer_ptr = std::shared_ptr<char>(new char[buffer_size], [](char*p) {delete[]p; });
					buffer_ptr.get()[0] = 1; buffer_ptr.get()[1] = 0; buffer_ptr.get()[2] = 0; buffer_ptr.get()[3] = 1;
					setPacketRemain(buffer_ptr, install_packet_name_.c_str(), static_cast<uint16_t>(install_packet_name_.length()));
					boost::asio::async_write(socket_, boost::asio::buffer(buffer_ptr.get(), buffer_size), yield);

					uint64_t offset = 0;
					char data[send_size_max - head_size] = { 0 };
					while (run_)
					{
						auto len = file_.async_read_some_at(offset, boost::asio::buffer(data, send_size_max - head_size), yield[ec]);
						if (!ec)
						{
							offset += len;
							auto buffer_size1 = head_size + len;
							auto buffer_ptr1 = std::shared_ptr<char>(new char[buffer_size1], [](char*p) {delete[]p; });
							buffer_ptr1.get()[0] = 1; buffer_ptr1.get()[1] = 0; buffer_ptr1.get()[2] = 0; buffer_ptr1.get()[3] = 2;
							setPacketRemain(buffer_ptr1, data, static_cast<uint16_t>(len));
							boost::asio::async_write(socket_, boost::asio::buffer(buffer_ptr1.get(), buffer_size1), yield);
						}
						else
						{
							if (ec.value() == 2 && ec.message() == "End of file")
							{
								char head[head_size] = { 0 };
								head[0] = 1; head[1] = 0; head[2] = 0; head[3] = 0;
								boost::asio::async_write(socket_, boost::asio::buffer(head, head_size), yield);
							}
							else
							{
								printf("%s", "read file error\n");
								return ;
							}	
						}
					}
				}
				catch (const std::exception& e)
				{
					auto str = e.what();
					file_.close();
					socket_.close();
				}
			});
		}

		Client(const Client& client) = delete;
		const Client& operator=(const Client&) = delete;
	private:
		void setPacketRemain(std::shared_ptr<char> send_buffer, const char* data_buffer, const uint16_t data_size)
		{	
			auto length_net = boost::asio::detail::socket_ops::host_to_network_short(static_cast<uint16_t>(data_size));
			std::memcpy(send_buffer.get() + 4, &length_net, sizeof(uint16_t));
			std::memcpy(send_buffer.get() + head_size, data_buffer, data_size);
		}
	};
}
#endif