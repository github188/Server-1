#pragma once
#include<list>
#include<mutex>
#include<thread>
#include <condition_variable>
#include <atomic>
namespace WwFoundation
{
	template<typename T>
	class SyncQueue
	{
	private:
		std::list<T> queue_;
		std::mutex mutex_;
		std::condition_variable not_empty_;
		std::condition_variable not_full_;
		unsigned int max_size_;
		std::atomic<bool> need_stop_;

	public:
		SyncQueue(int maxSize) :max_size_(maxSize), need_stop_(false) {}

		void put(const T& x)
		{
			add(x);
		}

		void put(T&&x)
		{
			add(std::forward<T>(x));
		}

		void take(std::list<T>& lis)
		{
			std::unique_lock<std::mutex> locker(mutex_);
			not_empty_.wait(locker, [this]() {return need_stop_ || notEmpty(); });
			if (need_stop_)
				return;

			lis = std::move(queue_);
			not_full_.notify_one();
		}

		void take(T& x)
		{
			std::unique_lock<std::mutex> locker(mutex_);
			not_empty_.wait(locker, [this]() {return need_stop_ || notEmpty(); });
			if (need_stop_)
				return;
			x = queue_.front();
			queue_.pop_front();
			not_full_.notify_one();
		}

		void stop()
		{
			need_stop_ = true;
			not_full_.notify_all();
			not_empty_.notify_all();
		}

		bool empty()
		{
			std::lock_guard<std::mutex> locker(mutex_);
			return queue_.empty();
		}

		bool full()
		{
			std::lock_guard<std::mutex> locker(mutex_);
			return queue_.size() == max_size_;
		}

		size_t size()
		{
			std::lock_guard<std::mutex> locker(mutex_);
			queue_.size();
		}

		int count()
		{
			return queue_.size();
		}
	private:
		bool notFull() const
		{
			bool full = (queue_.size() >= max_size_);
			return !full;
		}

		bool notEmpty() const
		{
			bool empty = queue_.empty();
			return !empty;
		}
	public:
		template<typename F>
		void add(F&& x)
		{
			std::unique_lock<std::mutex> locker(mutex_);
			//not_full_.wait(locker, [this]() {return need_stop_/*.load()*/ || notFull(); });
			if (need_stop_)
				return;

			while (queue_.size() >= max_size_) //remove old data,insert new data
			{
				queue_.front();
				queue_.pop_front();
			}
			queue_.emplace_back(std::forward<F>(x));
			not_empty_.notify_one();
		}
	};
}