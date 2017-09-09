#pragma once
#include <list>
#include <functional>
#include <memory>
#include <mutex>
#include <atomic>
#include <thread>
#include "TaskQueue.h"

namespace WwFoundation
{
	const int TASK_OK = 0;
	const int TASK_FAIL = -1;
	const int TASK_RETRY = 1;
	class ThreadPool
	{
	public:
		typedef  std::function<int()> Task;
	private:
		std::list<std::shared_ptr<std::thread>> thread_group_;
		WwFoundation::TaskQueue<Task> queue_;
		std::atomic<bool> running_;
		std::once_flag flag_;
	public:
		ThreadPool(int number_threads,int max_task_count) :queue_(max_task_count) { start(number_threads); }
	private:
		void start(int number_threads);
		void runThread();
		void stopThreadGroup();
		ThreadPool(const ThreadPool&) = delete;
		ThreadPool& operator=(const ThreadPool&) = delete;
	public:
		void stop();
		void addTask(Task&& task);
		void addTask(const Task& task);
	};
}
