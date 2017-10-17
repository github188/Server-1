#include <stdio.h>
#include <string.h>
#include <Windows.h>
#include "mysql.h"

void main(void)
{
	MYSQL *t_mysql;
	char select[] = "select statuScODE,ipv4,port from camerA";
	t_mysql = mysql_init(NULL);
	if (NULL == t_mysql)
		printf("init failed\n");

	if (NULL == mysql_real_connect(t_mysql, "192.168.1.103", "root", "111111", "platform", 20000, NULL, 0))
	{
		printf("connect failed :%s\n", mysql_error(t_mysql));
	}
	
	auto t = mysql_real_query(t_mysql, select, strlen(select));
	if (t)
	{
		printf("failed to query:%s\n", mysql_error(t_mysql));
		return;
	}
	printf("query success!\n");

	auto mysql_result = mysql_store_result(t_mysql);
	auto num_row = mysql_num_rows(mysql_result);
	auto num_col = mysql_num_fields(mysql_result);
	for (int i = 0; i<num_row;++i)
	{
		auto mysql_row = mysql_fetch_row(mysql_result);
		for (int j= 0; j<num_col; ++j)
		{
			printf("[Row %d,Col %d]==>[%s]\n", i, j, mysql_row[j]);
		}
	}
}
