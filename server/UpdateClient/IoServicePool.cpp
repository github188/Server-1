#include "IoServicePool.h"


update::client::IoServicePool::IoServicePool(int pool_size) :io_service_index_(0)
{
	for (int i = 0; i < pool_size; ++i)
	{
		auto io_service_ptr = std::make_shared<boost::asio::io_service>();
		auto work_ptr = std::make_shared<boost::asio::io_service::work>(*io_service_ptr);
		io_service_vec_.emplace_back(io_service_ptr);
		work_vec_.emplace_back(work_ptr);
	}
}

void update::client::IoServicePool::start()
{
	auto size = io_service_vec_.size();
	auto self = shared_from_this();
	for (int i = 0; i < static_cast<int>(size); ++i)
	{
		auto thread_ptr = std::make_shared<std::thread>([self, this, i]()
		{
			try
			{
				io_service_vec_[i]->run();
			}
			catch (std::exception& e)
			{
				printf("io_service thread with %s\n", e.what());
			}
			catch (...)
			{
				printf("io_service thread exit with unknown exception\n");
			}
		});
		thread_vec_.emplace_back(thread_ptr);
	}
}

void update::client::IoServicePool::stop()
{
	/*auto work_vec_size = work_vec_.size();
	for (int i = 0; i < static_cast<int>(work_vec_size); ++i)
	{
		work_vec_[i]->~work();
	}*/

	auto size = thread_vec_.size();
	for (int i = 0; i < static_cast<int>(size); ++i)
	{
		if (thread_vec_[i]->joinable())
			thread_vec_[i]->join();
	}
}

boost::asio::io_service & update::client::IoServicePool::get_io_service()
{
	auto& io_service = *(io_service_vec_[io_service_index_]);
	++io_service_index_;
	if (io_service_index_ == static_cast<int>(io_service_vec_.size()))
	{
		io_service_index_ = 0;
	}
	return io_service;
}
