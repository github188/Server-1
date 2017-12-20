#include "Session.h"
#include "boost/asio/spawn.hpp"
#include "boost/asio/write.hpp"

void update::client::Session::go()
{
	auto self(shared_from_this());
	boost::asio::spawn(socket_.get_io_service(), [this, self](boost::asio::yield_context yield)
	{
		try
		{
			//once send unsuccessfully,must do all again. All successfully or all unsuccessfully
			socket_.async_connect(endpoint_, yield); 
			int size = static_cast<int>(update_file_name_vec_.size());
			for (int i = 0; i < size; ++i)
			{
				auto update_file_name = update_file_name_vec_[i];
				boost::system::error_code ec;
				auto file_ptr = std::make_shared<boost::asio::windows::random_access_handle>(socket_.get_io_service());
				file_ptr->assign(CreateFile(update_file_name.c_str(), GENERIC_READ, 0, 0,
					OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 0), ec);
				file_vec_.emplace_back(file_ptr);
				auto buffer_size = HEAD_SIZE + update_file_name.length();
				auto buffer_ptr = std::shared_ptr<char>(new char[buffer_size], [](char*p) {delete[]p; });
				buffer_ptr.get()[0] = 1; buffer_ptr.get()[1] = 0; buffer_ptr.get()[2] = 0; buffer_ptr.get()[3] = 1;
				setPacketRemain(buffer_ptr, update_file_name.c_str(), static_cast<uint16_t>(update_file_name.length()));
				boost::asio::async_write(socket_, boost::asio::buffer(buffer_ptr.get(), buffer_size), yield);

				uint64_t offset = 0;
				char data[SEND_SIZE_MAX - HEAD_SIZE] = { 0 };
				for (;;)
				{
					auto len = file_ptr->async_read_some_at(offset, boost::asio::buffer(data, SEND_SIZE_MAX - HEAD_SIZE), yield[ec]);
					if (!ec)
					{
						offset += len;
						auto buffer_size1 = HEAD_SIZE + len;
						auto buffer_ptr1 = std::shared_ptr<char>(new char[buffer_size1], [](char*p) {delete[]p; });
						buffer_ptr1.get()[0] = 1; buffer_ptr1.get()[1] = 0; buffer_ptr1.get()[2] = 0; buffer_ptr1.get()[3] = 2;
						setPacketRemain(buffer_ptr1, data, static_cast<uint16_t>(len));
						boost::asio::async_write(socket_, boost::asio::buffer(buffer_ptr1.get(), buffer_size1), yield);
					}
					else
					{
						if (ec.value() == 2 && ec.message() == "End of file")
						{
							if (1 == size) //install packet
							{
								char head[HEAD_SIZE] = { 0 };
								head[0] = 1; head[1] = 0; head[2] = 0; head[3] = 3;
								boost::asio::async_write(socket_, boost::asio::buffer(head, HEAD_SIZE), yield);
							}
							else if ((i + 1) == size) //all file send out ok,binary replace
							{
								char head[HEAD_SIZE] = { 0 };
								head[0] = 1; head[1] = 0; head[2] = 0; head[3] = 4;
								boost::asio::async_write(socket_, boost::asio::buffer(head, HEAD_SIZE), yield);
							}
							break;
						}
						else
						{
							std::ostringstream message;
							message << "read file error with:" << ec.message();
							throw std::runtime_error(message.str());
						}
					}
				}
			}
		}
		catch (const std::exception& e)
		{
			auto str = e.what();
			boost::system::error_code ec;
			auto endpoint = socket_.remote_endpoint(ec);
			printf("%s,%s update failed! Please try again for next time", str, endpoint.address().to_string().c_str());
		}

		if (socket_.is_open())
			socket_.close();
		std::for_each(file_vec_.begin(), file_vec_.end(), 
			[](const std::shared_ptr<boost::asio::windows::random_access_handle> file_ptr)
		{
			if (file_ptr->is_open())
				file_ptr->close();
		});
	});
}

void update::client::Session::setPacketRemain(std::shared_ptr<char> send_buffer, const char * data_buffer, const uint16_t data_size)
{
	auto length_net = boost::asio::detail::socket_ops::host_to_network_short(static_cast<uint16_t>(data_size));
	std::memcpy(send_buffer.get() + 4, &length_net, sizeof(uint16_t));
	std::memcpy(send_buffer.get() + HEAD_SIZE, data_buffer, data_size);
}
