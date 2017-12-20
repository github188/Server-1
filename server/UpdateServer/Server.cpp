#include "Server.h"
#include <boost/asio/io_service.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/asio/ip/tcp.hpp>
#include "Session.h"
#include "ParseIni.h"

void update::server::Server::run()
{
	uint16_t port = 0;
	{
		WwFoundation::ParseIni parser(INI_DIR);
		parser.parse();
		port = static_cast<uint16_t>(std::atoi(parser.getValue("PortConfig", "port").c_str()));
	}
	boost::asio::io_service io_service;
	boost::asio::spawn(io_service, [&](boost::asio::yield_context yield)
	{
		boost::asio::ip::tcp::acceptor acceptor(io_service,
			boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port));
		for (;;)
		{
			boost::system::error_code ec;
			auto new_session = std::make_shared<session>(io_service);
			acceptor.async_accept(new_session->socket(), yield[ec]);
			if (!ec)
				new_session->go();
		}
	});
	io_service.run();
}
