#include "Session.h"
#include <boost/asio/spawn.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/write_at.hpp>
#include <boost/asio/read.hpp>
#include "LogMacros.h"
#include "Update.h"
#include "Update_installPacket.h"

void update::server::session::go()
{
	auto self = shared_from_this();
	boost::asio::spawn(socket_.get_io_service(), [this, self](boost::asio::yield_context yield)
	{
		try
		{
			char head[16] = { 0 };
			char data[1008] = { 0 };
			uint64_t offset = 0;
			std::string update_file_name = "";
			for (;;)
			{
				boost::asio::async_read(socket_, boost::asio::buffer(head, 16), yield);
				uint16_t data_legth_net = 0;
				std::memcpy(&data_legth_net, head + 4, sizeof(uint16_t));
				auto data_legth = boost::asio::detail::socket_ops::network_to_host_short(data_legth_net);
				if (head[0] == 1 && head[1] == 0 && head[2] == 0 && head[3] == 2) //content
				{
					boost::asio::async_read(socket_, boost::asio::buffer(data, data_legth), yield);
					auto ret = boost::asio::async_write_at(file_, offset, boost::asio::buffer(data, data_legth), yield);
					offset += data_legth;
				}
				else if (head[0] == 1 && head[1] == 0 && head[2] == 0 && head[3] == 1) //name
				{
					boost::asio::async_read(socket_, boost::asio::buffer(data, data_legth), yield);
					auto dwAttrib = GetFileAttributes(FILE_FOLDER.c_str());
					if (!(INVALID_FILE_ATTRIBUTES != dwAttrib && 0 != (dwAttrib & FILE_ATTRIBUTE_DIRECTORY)))
					{
						auto ret = CreateDirectory(FILE_FOLDER.c_str(), NULL);
						if (ret == 0)
						{
							std::ostringstream message;
							message << "CreateDirectory failed with error_code:" << GetLastError();
							throw std::runtime_error(message.str());
						}
					}

					std::string path_and_name = FILE_FOLDER;
					update_file_name = data;
					path_and_name.append(update_file_name);
					file_.assign(CreateFile(path_and_name.c_str(), GENERIC_WRITE, 0, 0,
						OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 0));
				}
				else if (head[0] == 1 && head[1] == 0 && head[2] == 0 && head[3] == 3) //can update now,update with installPacket
				{
					file_.close();
					socket_.close();
					UPDATESERVER_INFO("start update");
					auto update_ptr = std::make_shared<update::server::Update<UpdateInstallPacket>>(update_file_name);
					update_ptr->startUpdate();
				}
				else if (head[0] == 1 && head[1] == 0 && head[2] == 0 && head[3] == 4) //can update now,update with binaryReplace
				{
					file_.close();
					socket_.close();
					UPDATESERVER_INFO("start update");
					//update_share_ptr_ = std::make_shared<update::server::Update>(update_file_name);
					//update_share_ptr_->startUpdate();
				}
			}
		}
		catch (std::exception& e)
		{
			UPDATESERVER_ERROR("%s,coroutine exit", e.what());
		}
		catch (...)
		{
			UPDATESERVER_ERROR("coroutine exit with unknow exception!!");
		}
		if (file_.is_open())
			file_.close();
		if (socket_.is_open())
			socket_.close();
	});
}
