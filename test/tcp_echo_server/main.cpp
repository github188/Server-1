#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include "uv.h"

#define DEFAULT_PORT 7000
#define DEFAULT_BACKLOG 128
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib, "IPHLPAPI.lib")
#pragma comment(lib, "Psapi.lib")
#pragma comment(lib, "Userenv.lib")
const int WRITE_SIZE = 128;

int main()
{
	auto loop = uv_default_loop();
	uv_tcp_t server;
	uv_tcp_init(loop, &server);

	struct sockaddr_in addr;
	uv_ip4_addr("0.0.0.0", DEFAULT_PORT, &addr);

	uv_tcp_bind(&server, (const struct sockaddr*)&addr, 0);
	int r = uv_listen((uv_stream_t*)&server, DEFAULT_BACKLOG, [](uv_stream_t *server, int status)
	{
		if (status < 0)
		{
			fprintf(stderr, "New connection error %s\n", uv_strerror(status));
			return;
		}
		uv_tcp_t *client = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
		uv_tcp_init(server->loop, client);
		if (uv_accept(server, (uv_stream_t*)client) == 0)
		{
			//R"(C:/Users/ww/Desktop/iTS_Services_V1.0.234.26886_BNA_x86.exe)"
			auto fout_ptr = new std::ofstream(R"(C:/Users/ww/Desktop/iTS_Services_V1.0.234.26886_BNA_x86.exe)"
				, std::ios::binary | std::ios::app | std::ios::out);
			if (fout_ptr->bad())
			{
				uv_close((uv_handle_t*)client, NULL);
				return;
			}
			((uv_stream_t*)client)->data = fout_ptr;
			uv_read_start((uv_stream_t*)client, 
				[](uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) 
			{
				buf->base = (char*)malloc(suggested_size);
				buf->len = suggested_size;
			},
				[](uv_stream_t *client, ssize_t nread, const uv_buf_t *buf)
			{
				auto fout_ptr = (std::ofstream*)(client->data);
				if (nread > 0)
				{
					fout_ptr->write(buf->base, nread);
					printf("%i\n", nread);
				/*	uv_write((uv_write_t*)req, client, &req->buf, 1, [](uv_write_t *req, int status)
					{
						if (status)
						{
							fprintf(stderr, "Write error %s\n", uv_strerror(status));
						}
						write_req_t *wr = (write_req_t*)req;
						free(wr->buf.base);
						free(wr);
					});*/
				}
				if (nread < 0)
				{
					if (nread != UV_EOF)
						fprintf(stderr, "Read error %s\n", uv_err_name(nread));
					uv_close((uv_handle_t*)client, NULL);
					fout_ptr->close();
					delete fout_ptr;
				}
				free(buf->base);
			});
		}
		else
			uv_close((uv_handle_t*)client, NULL);
	});
	if (r)
	{
		fprintf(stderr, "Listen error %s\n", uv_strerror(r));
		return 1;
	}
	return uv_run(loop, UV_RUN_DEFAULT);
}