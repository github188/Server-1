#include <memory>
#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/write_at.hpp>
#include "boost/asio/read.hpp"
#include <boost/asio/windows/random_access_handle.hpp>

class session : public std::enable_shared_from_this<session>
{
private:
	boost::asio::ip::tcp::socket socket_;
	boost::asio::windows::random_access_handle file_;

public:
	explicit session(boost::asio::io_service& io_service) 
		:socket_(io_service),
		file_(io_service) {}
	~session(){printf("delete\n");}

	boost::asio::ip::tcp::socket& socket() { return socket_; }

	void go()
	{
		auto self = shared_from_this();
		boost::asio::spawn(socket_.get_io_service(), [this, self](boost::asio::yield_context yield)
		{
			try
			{
				char head[16] = { 0 };
				char data[1008] = { 0 };
				uint64_t offset = 0;
				for (;;)
				{
					boost::asio::async_read(socket_, boost::asio::buffer(head, 16), yield);
					uint16_t data_legth_net = 0;
					std::memcpy(&data_legth_net, head + 4, sizeof(uint16_t));
					auto data_legth = boost::asio::detail::socket_ops::network_to_host_short(data_legth_net);
					if (head[0] == 1 && head[1] == 0 && head[2] == 0 && head[3] == 2) //content
					{	
						boost::asio::async_read(socket_, boost::asio::buffer(data, data_legth), yield);
						auto ret = boost::asio::async_write_at(file_, offset, boost::asio::buffer(data, data_legth), yield);
						offset += data_legth;
					}
					else if (head[0] == 1 && head[1] == 0 && head[2] == 0 && head[3] == 1) //name
					{
						boost::asio::async_read(socket_, boost::asio::buffer(data, data_legth), yield);
						std::string path_and_name = "C:/Users/ww/Desktop/";
						path_and_name.append(data);
						boost::system::error_code ec;
						file_.assign(CreateFile(path_and_name.c_str(), GENERIC_WRITE, 0, 0,
							CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 0), ec);
					}
					else if (head[0] == 1 && head[1] == 0 && head[2] == 0 && head[3] == 0) //can update now
					{
						file_.close();
						socket_.close();
						STARTUPINFO si;
						PROCESS_INFORMATION pi;
						ZeroMemory(&pi, sizeof(pi));
						ZeroMemory(&si, sizeof(si));
						char cmdLine[] = "C:/Users/ww/Desktop/update.exe /verysilent";
						auto ret = CreateProcess(NULL, cmdLine, NULL, NULL, false, 0, NULL, NULL, &si, &pi);
						if (ret != 0)
						{
							CloseHandle(pi.hProcess);
							CloseHandle(pi.hThread);
							exit(0);
						}
					}
				}
			}
			catch (std::exception& e)
			{
				auto err_str = e.what();
				int zz;
			}
		});
	}
};

int main(int argc, char* argv[])
{
	try
	{
		boost::asio::io_service io_service;

		boost::asio::spawn(io_service, [&](boost::asio::yield_context yield)
		{
			boost::asio::ip::tcp::acceptor acceptor(io_service,
				boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 7000));
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
	catch (std::exception& e)
	{
		auto err_str = e.what();
		printf("%s\n", err_str);
	}
	return 0;
}
