#include "Client.h"
#include <thread>
#include "IoServicePool.h"
#include "Session.h"

void update::client::Client::addServerInstallMessage(const std::string & ip, const uint16_t port, 
	const std::vector<std::string>& update_file_name_vec)
{
	server_install_message_.emplace(ServerIpPort(ip, port), update_file_name_vec);
}

void update::client::Client::addServerInstallMessage(const std::string & ip, const uint16_t port, const std::string & update_file_name)
{
	std::vector<std::string> update_file_name_vec{ update_file_name };
	server_install_message_.emplace(ServerIpPort(ip, port), update_file_name_vec);
}

void update::client::Client::run()
{
	int cpu_size = std::thread::hardware_concurrency();
	io_service_pool_ptr_ = std::make_shared<IoServicePool>(cpu_size);
	io_service_pool_ptr_->start();

	std::for_each(server_install_message_.begin(), server_install_message_.end(),
		[this](auto& pair)
	{
		session_ptr_ = std::make_shared<Session>(io_service_pool_ptr_->get_io_service(), pair.first.ip,
			pair.first.port, pair.second);
		session_ptr_->go();
	});
}

void update::client::Client::stop()
{
	io_service_pool_ptr_->stop();
}
