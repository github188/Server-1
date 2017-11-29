#include <memory>
#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/windows/random_access_handle.hpp>

class session : public std::enable_shared_from_this<session>
{
private:
	boost::asio::windows::random_access_handle file_;
	boost::asio::ip::tcp::socket socket_;
public:
	explicit session(boost::asio::io_service& io_service) 
		:socket_(io_service),
		file_(io_service) {}
	~session(){printf("delete\n");}

	boost::asio::ip::tcp::socket& socket() { return socket_; }

	void go()
	{
		boost::system::error_code ec;
		file_.assign(CreateFile(L"C:/Users/ww/Desktop/update.exe", GENERIC_WRITE, 0, 0,
			CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 0), ec);
		auto self = shared_from_this();
		boost::asio::spawn(socket_.get_io_service(), [this, self](boost::asio::yield_context yield)
		{
			try
			{
				char data[1024 * 8] = { 0 };
				uint64_t offset = 0;
				for (;;)
				{
					auto n = socket_.async_read_some(boost::asio::buffer(data), yield);
					file_.async_write_some_at(offset, boost::asio::buffer(data, n), yield);
					offset += n;
				}
			}
			catch (std::exception& e)
			{
				auto err_str = e.what();
				file_.close();
				socket_.close();
				STARTUPINFO si;
				PROCESS_INFORMATION pi;
				ZeroMemory(&pi, sizeof(pi));
				ZeroMemory(&si, sizeof(si));
				wchar_t cmdLine[] = L"C:/Users/ww/Desktop/update.exe /verysilent";
				auto ret = CreateProcess(NULL, cmdLine, NULL, NULL, false, 0, NULL, NULL, &si, &pi);
				if (ret != 0)
				{
					CloseHandle(pi.hProcess);
					CloseHandle(pi.hThread);
					exit(0);
				}
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
