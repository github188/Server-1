#pragma once
#ifndef SERVER_SERVER_UPDATESERVER_SESSION_H
#define SERVER_SERVER_UPDATESERVER_SESSION_H
#include <memory>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/windows/random_access_handle.hpp>

//namespace update { namespace server { template<typename UpdateMethod> class Update; } }
namespace update
{
	namespace server
	{
		class session : public std::enable_shared_from_this<session>
		{
		private:
			boost::asio::ip::tcp::socket socket_;
			boost::asio::windows::random_access_handle file_;
		public:
			explicit session(boost::asio::io_service& io_service) :socket_(io_service), file_(io_service) {}
			~session() { printf("delete\n"); }
			boost::asio::ip::tcp::socket& socket() { return socket_; }
			void go();
		};
	}
}
#endif

