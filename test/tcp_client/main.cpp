#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread>
#include <fstream>
#include <fcntl.h>
#include "uv.h"

#define DEFAULT_PORT 7000
#define READ_SZIE 1024
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib, "IPHLPAPI.lib")
#pragma comment(lib, "Psapi.lib")
#pragma comment(lib, "Userenv.lib")

typedef struct my_uv_data
{
	uv_file file_index;
	uv_buf_t iov;
	uv_fs_t* open_req;
	uv_fs_t* read_req;
}MyUvData;

FILE* file_test;

void write_callback(uv_write_t* req, int status);
void fs_read_callback(uv_fs_t* req) 
{
	auto my_uv_data_ptr = (MyUvData*)req->data;
	if (req->result > 0)
	{
		my_uv_data_ptr->iov.len = req->result;
		my_uv_data_ptr->read_req = req;
		auto write_req = new uv_write_t;
		write_req->data = my_uv_data_ptr;
		uv_write(write_req, static_cast<uv_connect_t*>(my_uv_data_ptr->open_req->data)->handle, &my_uv_data_ptr->iov, 1, write_callback);
		Sleep(0);
	}
	else if (req->result == 0)
	{
		uv_fs_t close_req;
		uv_fs_close(uv_default_loop(), &close_req, my_uv_data_ptr->file_index, nullptr);
		delete static_cast<uv_connect_t*>(my_uv_data_ptr->open_req->data);
		uv_fs_req_cleanup(my_uv_data_ptr->open_req);
		delete my_uv_data_ptr->open_req;
		delete my_uv_data_ptr->iov.base;
		delete my_uv_data_ptr;
		uv_fs_req_cleanup(req);
		delete req;
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
	else if (req->result < 0)
		printf("error read file :%s\n", uv_strerror(req->result));
}
void write_callback(uv_write_t* req, int status)
{
	auto my_uv_data_ptr = (MyUvData*)req->data;
	if (status)
		printf("write error %s\n", uv_strerror(status));
	else
		uv_fs_read(uv_default_loop(), my_uv_data_ptr->read_req, my_uv_data_ptr->file_index, &my_uv_data_ptr->iov, 1, -1, fs_read_callback);
	delete req;
}


int main()
{
	//file_test = fopen("C:/Users/ww/Desktop/test.exe", "ab+");

	uv_tcp_t client;
	uv_tcp_init(uv_default_loop(), &client);

	struct sockaddr_in dest;
	uv_ip4_addr("192.168.1.182", DEFAULT_PORT, &dest);

	auto req = new uv_connect_t;
	int r = uv_tcp_connect(req, &client, (struct sockaddr*)&dest, [](uv_connect_t* req, int status)
	{
		if (status < 0)
		{
			printf("connect error %s\n", uv_strerror(status));
			delete req;
			return;
		}

		auto open_req = new uv_fs_t;
		open_req->data = req;
		uv_fs_open(uv_default_loop(), open_req, "C:/Users/ww/Desktop/iTS_Services_V1.0.234.26886_BNA_x86.exe", O_RDONLY, 0, [](uv_fs_t* req)
		{
			if (req->result >= 0)
			{
				auto base = new char[READ_SZIE];;
				auto my_uv_data_ptr = new MyUvData;
				my_uv_data_ptr->file_index = req->result;
				my_uv_data_ptr->iov.len = READ_SZIE;
				my_uv_data_ptr->iov.base = base;
				my_uv_data_ptr->open_req = req;
				auto read_req = new uv_fs_t;
				read_req->data = my_uv_data_ptr;
				uv_fs_read(uv_default_loop(), read_req, req->result, &my_uv_data_ptr->iov, 1, -1, fs_read_callback);
			}
			else
				printf("error open file :%s\n", uv_strerror(req->result));		
		});
		

		//R"(C:/Users/ww/Desktop/iTS_Services_V1.0.234.26886_BNA_x86.exe)"
	//	std::ifstream fin(R"(C:/Users/ww/Desktop/iTS_Services_V1.0.234.26886_BNA_x86.exe)", std::ios::binary | std::ios::in);
	//	if (fin.bad())
	//		return;

	//	uv_buf_t buf;
	//	char szBuf[READ_SZIE] = { 0 };
	////	while (!fin.eof())
	//	{
	//		fin.read(szBuf, sizeof(char)*READ_SZIE);
	//		buf.base = szBuf;
	//		buf.len = READ_SZIE;

	//		auto write_req = new uv_write_t;
	//		uv_write(write_req, req->handle, &buf, 1, [](uv_write_t* req, int status) 
	//		{
	//			delete req;
	//			printf("11\n");
	//		});
	//		std::this_thread::sleep_for(std::chrono::milliseconds(5));
	//	}
	//	fin.close();
	});
	if (r)
	{
		fprintf(stderr, "Listen error %s\n", uv_strerror(r));
		return 1;
	}
	return uv_run(uv_default_loop(), UV_RUN_DEFAULT);
}