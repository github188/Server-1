#include "DbWriter.h"
#define OTL_ORA12C_R2 // Compile OTL 4.0/OCI12
#define OTL_STL // Enable STL compatibility mode
#define OTL_ORA_UTF8
#define OTL_STREAM_POOLING_ON
#define OTL_ORA_TIMESTAMP
#include "otlv4_h2/otlv4.h"
#include "SyncQueue.h"
void WwFoundation::DbWriter::start(int number_threads)
{
	running_ = true;
	for (int i = 0; i < number_threads; ++i)
	{
		thread_group_.emplace_back(std::make_shared<std::thread>(&DbWriter::runThread, this));
	}
	thread_alive_count_ = number_threads;
}

void WwFoundation::DbWriter::runThread()
{
	try
	{
		otl_connect::otl_initialize(true); // initialize OCI environment,mutiple thread enable,but not safety
		auto db_ptr = std::make_shared<otl_connect>();

		unsigned int count = 0;
		bool need_rlogon = true;
		while (running_)
		{
			std::list<Task> list;
			queue_.take(list);
			for (auto& task : list)
			{
				if (!running_)
					return;
				while (running_)
				{
					try
					{
						if (need_rlogon)
						{
							db_ptr->rlogon(connect_str_.c_str());
							need_rlogon = false;
						}
						otl_nocommit_stream o1(1, task.c_str(), *db_ptr); //insert one record. if 2 ,insert two same records.....
						o1.close(true);
						break;
					}
					catch (otl_exception& p)// intercept OTL exceptions. ex:ORA-12543: TNS:destination host unreachable
					{
						printf("%s", p.msg);// error message
						printf("%s", p.stm_text);// SQL that caused the error
						printf("%s", p.var_info);//the variable that caused the error
						if (db_ptr->connected == 1)
							db_ptr->logoff();
						std::this_thread::sleep_for(std::chrono::milliseconds(retry_rate));
						need_rlogon = true;
					}
				}	
			}
			count = count + list.size();
			if (count >= commit_size_)
			{
				db_ptr->commit();
				count = 0;
			}
		}
		db_ptr->commit();
	}
	catch (const std::exception& e)
	{
		printf("%s, DbWriter::runThread exit", e.what());
		thread_alive_count_ = thread_alive_count_ - 1;
	}
	catch (...)
	{
		printf("DbWriter::runThread exit with unknown exception!!");
		thread_alive_count_ = thread_alive_count_ - 1;
	}
}

void WwFoundation::DbWriter::stop()
{
	std::call_once(flag_, [this]() {stopThreadGroup(); });
}

void WwFoundation::DbWriter::stopThreadGroup()
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

void WwFoundation::DbWriter::addSql(Task && task)
{
	queue_.put(std::forward<Task>(task));
}

void WwFoundation::DbWriter::addSql(const Task & task)
{
	queue_.put(task);
}
