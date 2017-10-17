#pragma once
#include <list>
#include <thread>
#include <functional>
#include <memory>
#include <atomic>
#include <vector>
#include <string>
#include "SyncQueue.h"

int const retry_rate = 30000;//ms
namespace WwFoundation
{
	class DbWriter
	{
		using Task = std::string;
	private:
		std::list<std::shared_ptr<std::thread>> thread_group_;
		SyncQueue<Task> queue_;
		std::atomic<bool> running_;
		std::once_flag flag_;
		std::string connect_str_;
		unsigned int commit_size_;
		std::atomic<int> thread_alive_count_;
	public:
		DbWriter(int number_threads, int max_sql_count, int commit_size, const std::string connect_str) 
			:queue_(max_sql_count),commit_size_(commit_size), connect_str_(connect_str)
		{ start(number_threads); }
		~DbWriter() { stop(); }
		DbWriter(const DbWriter&) = delete;
		DbWriter& operator=(const DbWriter&) = delete;
	private:
		void start(int number_threads);
		void runThread();
		void stopThreadGroup();
	public:
		void stop();
		void addSql(Task&& task);
		void addSql(const Task& task);
		int getThreadAliveCount() const { return thread_alive_count_; }
	};
}
