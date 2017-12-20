#pragma once
#ifndef SERVER_CLIENT_UPDATECLIENT_IOSERVICEPOOL_H
#define SERVER_CLIENT_UPDATECLIENT_IOSERVICEPOOL_H
#include <vector>
#include <memory>
#include <thread>
#include "boost/asio/io_service.hpp"

namespace update
{
	namespace client
	{
		class IoServicePool :public std::enable_shared_from_this<IoServicePool>
		{
		private:
			int io_service_index_;
			std::vector<std::shared_ptr<boost::asio::io_service>> io_service_vec_;
			std::vector<std::shared_ptr<boost::asio::io_service::work>> work_vec_; //keep io_service.run() alive
			std::vector<std::shared_ptr<std::thread>> thread_vec_;
		public:
			explicit IoServicePool(int pool_size);
			void start();
			void stop();
			boost::asio::io_service& get_io_service();
		private:
			IoServicePool(const IoServicePool&) = delete;
			IoServicePool& operator=(const IoServicePool&) = delete;
		};
	}
}
#endif