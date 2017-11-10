#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread>
#include <fstream>
#include "uv.h"


#define DEFAULT_PORT 7000
#define READ_SZIE 131070
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib, "IPHLPAPI.lib")
#pragma comment(lib, "Psapi.lib")
#pragma comment(lib, "Userenv.lib")

int main()
{
	auto loop = uv_default_loop();
	uv_tcp_t client;
	uv_tcp_init(loop, &client);

	struct sockaddr_in dest;
	uv_ip4_addr("192.168.1.158", DEFAULT_PORT, &dest);

	auto req = new uv_connect_t;
	int r = uv_tcp_connect(req, &client, (struct sockaddr*)&dest, [](uv_connect_t* req, int status) 
	{
		if (status < 0)
		{
			printf("connect error %s\n", uv_strerror(status));
			return;
		}
		//R"(C:/Users/ww/Desktop/iTS_Services_V1.0.234.26886_BNA_x86.exe)"
		std::ifstream fin(R"(C:/Users/ww/Desktop/iTS_Services_V1.0.234.26886_BNA_x86.exe)", std::ios::binary | std::ios::in);
		if (fin.bad())
			return;

		uv_buf_t buf;
		char szBuf[READ_SZIE] = { 0 };
		while (!fin.eof())
		{
			fin.read(szBuf, sizeof(char)*READ_SZIE);
			buf.base = szBuf;
			buf.len = READ_SZIE;

			auto write_req = new uv_write_t;
			uv_write(write_req, req->handle, &buf, 1, [](uv_write_t* req, int status) 
			{
				delete req;
				printf("11\n");
			});
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
		}
		fin.close();
	});
	if (r)
	{
		fprintf(stderr, "Listen error %s\n", uv_strerror(r));
		return 1;
	}
	return uv_run(loop, UV_RUN_DEFAULT);
}