#include "ThreadPool.h"
#include "TaskQueue.h"
#include "LogMacros.h"

void WwFoundation::ThreadPool::start(int number_threads)
{
	running_ = true;
	for (int i = 0; i < number_threads; ++i)
	{
		thread_group_.emplace_back(std::make_shared<std::thread>(&ThreadPool::runThread, this));
	}
}

void WwFoundation::ThreadPool::runThread()
{
	try
	{
		while (running_)
		{
			/*std::list<Task> list;
			queue_.take(list);
			for (auto task = list.begin(); task != list.end(); ++task)
			{
				if (!running_)
					return;
				(*task)();
			}*/
			Task task;
			queue_.take(task);
			if (!running_)
				return;
			if (task)
			{
				auto task_result = task();
				if (task_result == TASK_RETRY)
					queue_.add(task);
			}
		}
	}
	catch (const std::exception& e)
	{
		VIDEODETECTSERVER_ERROR("thread pool runThread exit:%s", e.what());
	}
	catch (...)
	{
		VIDEODETECTSERVER_ERROR("thread pool runThread exit with unknown exception");
	}
}

void WwFoundation::ThreadPool::stop()
{
	std::call_once(flag_, [this]() {stopThreadGroup(); });
}

void WwFoundation::ThreadPool::stopThreadGroup()
{
	queue_.stop();
	running_ = false;
	for (auto thread : thread_group_)
	{
		if (thread->joinable())
			thread->join();
	}
	thread_group_.clear();
}

void WwFoundation::ThreadPool::addTask(Task && task)
{
	queue_.put(std::forward<Task>(task));
}

void WwFoundation::ThreadPool::addTask(const Task & task)
{
	queue_.put(task);
}
