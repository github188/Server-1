#include "Client.h"
#include <thread>
#include "IoServicePool.h"
#include "Session.h"

void update::client::Client::makeSession(const std::string & ip, const uint16_t port,
	const std::vector<std::string>& update_file_name_vec)
{
	auto session_ptr_ = std::make_shared<Session>(io_service_pool_ptr_->get_io_service(), ip,
		port, update_file_name_vec);
	session_ptr_->go();
}

void update::client::Client::makeSession(const std::string & ip, const uint16_t port, 
	const std::string & update_file_name)
{
	std::vector<std::string> update_file_name_vec{ update_file_name };
	auto session_ptr_ = std::make_shared<Session>(io_service_pool_ptr_->get_io_service(), ip,
		port, update_file_name_vec);
	session_ptr_->go();
}

void update::client::Client::run()
{
	int cpu_size = std::thread::hardware_concurrency();
	io_service_pool_ptr_ = std::make_shared<IoServicePool>(cpu_size);
	io_service_pool_ptr_->start();
}

void update::client::Client::stop()
{
	io_service_pool_ptr_->stop();
}
