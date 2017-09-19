#include <ctime>
#include <string>
#include <sstream>
#include <memory>
#include <thread>
#pragma warning(disable: 4996)
#include "DbWriter.h"
#include "ParseIni.h"
#include "RAII.h"
// 2 crossing , 16 cameras, 32 lanes ,so 16 threads to write data
const std::string CAMERA_NUMBER = "camera_number";
const std::string CAMERA_LANE_NUMBER = "lane_number";
const std::string CAMERA_CAR_FLOW = "car_flow";
const std::string CAMERA_UPLOAD_TIME = "upload_time";
const std::string CAMERA_RAW_LANE_NUMBER = "raw_lane_number";
const int sleep_rate = 15;//ms
const int commit_size = 10000;

typedef struct configmessage
{
	std::string db_UserName;
	std::string db_ip;
	std::string db_port;
	std::string db_name;
	std::string db_password;
	std::string deviceTable_name;
	std::string videoDetectDataTable_name;
	int max_peak_carFlow;
	int max_low_carFlow;
	int days;
}ConfigMessage;


int main()
{
	ConfigMessage cm_;
	WwFoundation::ParseIni parser("MakeVideoDetectData.ini");
	parser.parse();
	cm_.db_UserName = parser.getValue("DbConfig", "DbUserName");
	cm_.db_ip = parser.getValue("DbConfig", "DbIp");
	cm_.db_name = parser.getValue("DbConfig", "DbName");
	cm_.db_password = parser.getValue("DbConfig", "DbPassword");
	cm_.db_port = parser.getValue("DbConfig", "DbPort");
	cm_.videoDetectDataTable_name = parser.getValue("DbConfig", "VideoDetectDataTableName");
	cm_.deviceTable_name = parser.getValue("DbConfig", "DeviceTableName");
	cm_.max_peak_carFlow = std::atoi(parser.getValue("ComConfig", "MaxPeakCarFlow").c_str());
	cm_.max_peak_carFlow = cm_.max_peak_carFlow >= 1 ? cm_.max_peak_carFlow : 1;
	cm_.max_low_carFlow = std::atoi(parser.getValue("ComConfig", "MaxLowCarFlow").c_str());
	cm_.max_low_carFlow = cm_.max_low_carFlow >= 1 ? cm_.max_low_carFlow : 1;
	cm_.days = std::atoi(parser.getValue("ComConfig", "Day").c_str());

	auto time_now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()); //now time(second)
	std::ostringstream message;
	message << cm_.db_UserName << "/" << cm_.db_password << "@" <<
		cm_.db_ip << ":" << cm_.db_port << "/" << cm_.db_name;

	std::thread thread1([&time_now, &cm_,&message]()
	{
		auto dbWriter_ptr = std::make_shared<WwFoundation::DbWriter>(1, 500000, commit_size, message.str());
		std::vector<std::string> lane_number{ "501","502" };
		auto time_after_year = time_now + (cm_.days * 24 * 3600);
		std::ostringstream time_temp;
		std::ostringstream sql;
		for (auto t = time_now; t <= time_after_year;)
		{
			time_temp.str("");
			time_temp.clear();
			time_temp << (std::localtime(&t)->tm_year + 1900)
				<< "-" << (std::localtime(&t)->tm_mon + 1)
				<< "-" << std::localtime(&t)->tm_mday
				<< " " << std::localtime(&t)->tm_hour
				<< ":" << std::localtime(&t)->tm_min
				<< ":" << std::localtime(&t)->tm_sec;

			for (int i = 0; i < 2; ++i) //2 lanes
			{
				sql.str("");
				sql.clear();
				sql << "insert into " << cm_.videoDetectDataTable_name
					<< "(" << CAMERA_NUMBER
					<< "," << CAMERA_RAW_LANE_NUMBER
					<< "," << CAMERA_UPLOAD_TIME
					<< "," << CAMERA_LANE_NUMBER
					<< "," << CAMERA_CAR_FLOW << ")"
					<< " values ("
					<< "'" << "thread1" << "'"
					<< "," << "'" << i + 1 << "'"
					<< "," << "to_date('" << time_temp.str() << "','YYYY-MM-DD HH24:MI:SS')"
					<< "," << "'" << lane_number[i] << "'";

				auto hour = std::localtime(&t)->tm_hour;
				if ((hour > 7 && hour < 10) || (hour > 17 && hour < 20)) //7-10 or 17-20 ,more cars
				{
					auto car_flow = std::rand() % cm_.max_peak_carFlow;
					sql << "," << "'" << car_flow << "'" << ")";
				}
				else
				{
					auto car_flow = std::rand() % cm_.max_low_carFlow;
					sql << "," << "'" << car_flow << "'" << ")";
				}
				dbWriter_ptr->addSql(sql.str());
			}
			t = t + 60; // each minute
			std::this_thread::sleep_for(std::chrono::milliseconds(sleep_rate));
		}
		printf("thread1 exit\n");
	});

	std::thread thread2([&time_now, &cm_, &message]()
	{
		auto dbWriter_ptr = std::make_shared<WwFoundation::DbWriter>(1, 500000, commit_size, message.str());
		std::vector<std::string> lane_number{ "503","504" };
		auto time_after_year = time_now + (cm_.days * 24 * 3600);
		std::ostringstream time_temp;
		std::ostringstream sql;
		for (auto t = time_now; t <= time_after_year;)
		{
			time_temp.str("");
			time_temp.clear();
			time_temp << (std::localtime(&t)->tm_year + 1900)
				<< "-" << (std::localtime(&t)->tm_mon + 1)
				<< "-" << std::localtime(&t)->tm_mday
				<< " " << std::localtime(&t)->tm_hour
				<< ":" << std::localtime(&t)->tm_min
				<< ":" << std::localtime(&t)->tm_sec;

			for (int i = 0; i < 2; ++i) //2 lanes
			{
				sql.str("");
				sql.clear();
				sql << "insert into " << cm_.videoDetectDataTable_name
					<< "(" << CAMERA_NUMBER
					<< "," << CAMERA_RAW_LANE_NUMBER
					<< "," << CAMERA_UPLOAD_TIME
					<< "," << CAMERA_LANE_NUMBER
					<< "," << CAMERA_CAR_FLOW << ")"
					<< " values ("
					<< "'" << "thread2" << "'"
					<< "," << "'" << i + 1 << "'"
					<< "," << "to_date('" << time_temp.str() << "','YYYY-MM-DD HH24:MI:SS')"
					<< "," << "'" << lane_number[i] << "'";

				auto hour = std::localtime(&t)->tm_hour;
				if ((hour > 7 && hour < 10) || (hour > 17 && hour < 20)) //7-10 or 17-20 ,more cars
				{
					auto car_flow = std::rand() % cm_.max_peak_carFlow;
					sql << "," << "'" << car_flow << "'" << ")";
				}
				else
				{
					auto car_flow = std::rand() % cm_.max_low_carFlow;
					sql << "," << "'" << car_flow << "'" << ")";
				}
				dbWriter_ptr->addSql(sql.str());
			}
			t = t + 60; // each minute
			std::this_thread::sleep_for(std::chrono::milliseconds(sleep_rate));
		}
		printf("thread2 exit\n");
	});

	std::thread thread3([&time_now, &cm_, &message]()
	{
		auto dbWriter_ptr = std::make_shared<WwFoundation::DbWriter>(1, 500000, commit_size, message.str());
		std::vector<std::string> lane_number{ "505","506" };
		auto time_after_year = time_now + (cm_.days * 24 * 3600);
		std::ostringstream time_temp;
		std::ostringstream sql;
		for (auto t = time_now; t <= time_after_year;)
		{
			time_temp.str("");
			time_temp.clear();
			time_temp << (std::localtime(&t)->tm_year + 1900)
				<< "-" << (std::localtime(&t)->tm_mon + 1)
				<< "-" << std::localtime(&t)->tm_mday
				<< " " << std::localtime(&t)->tm_hour
				<< ":" << std::localtime(&t)->tm_min
				<< ":" << std::localtime(&t)->tm_sec;

			for (int i = 0; i < 2; ++i) //2 lanes
			{
				sql.str("");
				sql.clear();
				sql << "insert into " << cm_.videoDetectDataTable_name
					<< "(" << CAMERA_NUMBER
					<< "," << CAMERA_RAW_LANE_NUMBER
					<< "," << CAMERA_UPLOAD_TIME
					<< "," << CAMERA_LANE_NUMBER
					<< "," << CAMERA_CAR_FLOW << ")"
					<< " values ("
					<< "'" << "thread3" << "'"
					<< "," << "'" << i + 1 << "'"
					<< "," << "to_date('" << time_temp.str() << "','YYYY-MM-DD HH24:MI:SS')"
					<< "," << "'" << lane_number[i] << "'";

				auto hour = std::localtime(&t)->tm_hour;
				if ((hour > 7 && hour < 10) || (hour > 17 && hour < 20)) //7-10 or 17-20 ,more cars
				{
					auto car_flow = std::rand() % cm_.max_peak_carFlow;
					sql << "," << "'" << car_flow << "'" << ")";
				}
				else
				{
					auto car_flow = std::rand() % cm_.max_low_carFlow;
					sql << "," << "'" << car_flow << "'" << ")";
				}
				dbWriter_ptr->addSql(sql.str());
			}
			t = t + 60; // each minute
			std::this_thread::sleep_for(std::chrono::milliseconds(sleep_rate));
		}
		printf("thread3 exit\n");
	});

	std::thread thread4([&time_now, &cm_, &message]()
	{
		auto dbWriter_ptr = std::make_shared<WwFoundation::DbWriter>(1, 500000, commit_size, message.str());
		std::vector<std::string> lane_number{ "507","508" };
		auto time_after_year = time_now + (cm_.days * 24 * 3600);
		std::ostringstream time_temp;
		std::ostringstream sql;
		for (auto t = time_now; t <= time_after_year;)
		{
			time_temp.str("");
			time_temp.clear();
			time_temp << (std::localtime(&t)->tm_year + 1900)
				<< "-" << (std::localtime(&t)->tm_mon + 1)
				<< "-" << std::localtime(&t)->tm_mday
				<< " " << std::localtime(&t)->tm_hour
				<< ":" << std::localtime(&t)->tm_min
				<< ":" << std::localtime(&t)->tm_sec;

			for (int i = 0; i < 2; ++i) //2 lanes
			{
				sql.str("");
				sql.clear();
				sql << "insert into " << cm_.videoDetectDataTable_name
					<< "(" << CAMERA_NUMBER
					<< "," << CAMERA_RAW_LANE_NUMBER
					<< "," << CAMERA_UPLOAD_TIME
					<< "," << CAMERA_LANE_NUMBER
					<< "," << CAMERA_CAR_FLOW << ")"
					<< " values ("
					<< "'" << "thread4" << "'"
					<< "," << "'" << i + 1 << "'"
					<< "," << "to_date('" << time_temp.str() << "','YYYY-MM-DD HH24:MI:SS')"
					<< "," << "'" << lane_number[i] << "'";

				auto hour = std::localtime(&t)->tm_hour;
				if ((hour > 7 && hour < 10) || (hour > 17 && hour < 20)) //7-10 or 17-20 ,more cars
				{
					auto car_flow = std::rand() % cm_.max_peak_carFlow;
					sql << "," << "'" << car_flow << "'" << ")";
				}
				else
				{
					auto car_flow = std::rand() % cm_.max_low_carFlow;
					sql << "," << "'" << car_flow << "'" << ")";
				}
				dbWriter_ptr->addSql(sql.str());
			}
			t = t + 60; // each minute
			std::this_thread::sleep_for(std::chrono::milliseconds(sleep_rate));
		}
		printf("thread4 exit\n");
	});

	std::thread thread5([&time_now, &cm_, &message]()
	{
		auto dbWriter_ptr = std::make_shared<WwFoundation::DbWriter>(1, 500000, commit_size, message.str());
		std::vector<std::string> lane_number{ "509","510" };
		auto time_after_year = time_now + (cm_.days * 24 * 3600);
		std::ostringstream time_temp;
		std::ostringstream sql;
		for (auto t = time_now; t <= time_after_year;)
		{
			time_temp.str("");
			time_temp.clear();
			time_temp << (std::localtime(&t)->tm_year + 1900)
				<< "-" << (std::localtime(&t)->tm_mon + 1)
				<< "-" << std::localtime(&t)->tm_mday
				<< " " << std::localtime(&t)->tm_hour
				<< ":" << std::localtime(&t)->tm_min
				<< ":" << std::localtime(&t)->tm_sec;

			for (int i = 0; i < 2; ++i) //2 lanes
			{
				sql.str("");
				sql.clear();
				sql << "insert into " << cm_.videoDetectDataTable_name
					<< "(" << CAMERA_NUMBER
					<< "," << CAMERA_RAW_LANE_NUMBER
					<< "," << CAMERA_UPLOAD_TIME
					<< "," << CAMERA_LANE_NUMBER
					<< "," << CAMERA_CAR_FLOW << ")"
					<< " values ("
					<< "'" << "thread5" << "'"
					<< "," << "'" << i + 1 << "'"
					<< "," << "to_date('" << time_temp.str() << "','YYYY-MM-DD HH24:MI:SS')"
					<< "," << "'" << lane_number[i] << "'";

				auto hour = std::localtime(&t)->tm_hour;
				if ((hour > 7 && hour < 10) || (hour > 17 && hour < 20)) //7-10 or 17-20 ,more cars
				{
					auto car_flow = std::rand() % cm_.max_peak_carFlow;
					sql << "," << "'" << car_flow << "'" << ")";
				}
				else
				{
					auto car_flow = std::rand() % cm_.max_low_carFlow;
					sql << "," << "'" << car_flow << "'" << ")";
				}
				dbWriter_ptr->addSql(sql.str());
			}
			t = t + 60; // each minute
			std::this_thread::sleep_for(std::chrono::milliseconds(sleep_rate));
		}
		printf("thread5 exit\n");
	});

	std::thread thread6([&time_now, &cm_, &message]()
	{
		auto dbWriter_ptr = std::make_shared<WwFoundation::DbWriter>(1, 500000, commit_size, message.str());
		std::vector<std::string> lane_number{ "511","512" };
		auto time_after_year = time_now + (cm_.days * 24 * 3600);
		std::ostringstream time_temp;
		std::ostringstream sql;
		for (auto t = time_now; t <= time_after_year;)
		{
			time_temp.str("");
			time_temp.clear();
			time_temp << (std::localtime(&t)->tm_year + 1900)
				<< "-" << (std::localtime(&t)->tm_mon + 1)
				<< "-" << std::localtime(&t)->tm_mday
				<< " " << std::localtime(&t)->tm_hour
				<< ":" << std::localtime(&t)->tm_min
				<< ":" << std::localtime(&t)->tm_sec;

			for (int i = 0; i < 2; ++i) //2 lanes
			{
				sql.str("");
				sql.clear();
				sql << "insert into " << cm_.videoDetectDataTable_name
					<< "(" << CAMERA_NUMBER
					<< "," << CAMERA_RAW_LANE_NUMBER
					<< "," << CAMERA_UPLOAD_TIME
					<< "," << CAMERA_LANE_NUMBER
					<< "," << CAMERA_CAR_FLOW << ")"
					<< " values ("
					<< "'" << "thread6" << "'"
					<< "," << "'" << i + 1 << "'"
					<< "," << "to_date('" << time_temp.str() << "','YYYY-MM-DD HH24:MI:SS')"
					<< "," << "'" << lane_number[i] << "'";

				auto hour = std::localtime(&t)->tm_hour;
				if ((hour > 7 && hour < 10) || (hour > 17 && hour < 20)) //7-10 or 17-20 ,more cars
				{
					auto car_flow = std::rand() % cm_.max_peak_carFlow;
					sql << "," << "'" << car_flow << "'" << ")";
				}
				else
				{
					auto car_flow = std::rand() % cm_.max_low_carFlow;
					sql << "," << "'" << car_flow << "'" << ")";
				}
				dbWriter_ptr->addSql(sql.str());
			}
			t = t + 60; // each minute
			std::this_thread::sleep_for(std::chrono::milliseconds(sleep_rate));
		}
		printf("thread6 exit\n");
	});

	std::thread thread7([&time_now, &cm_, &message]()
	{
		auto dbWriter_ptr = std::make_shared<WwFoundation::DbWriter>(1, 500000, commit_size, message.str());
		std::vector<std::string> lane_number{ "513","514" };
		auto time_after_year = time_now + (cm_.days * 24 * 3600);
		std::ostringstream time_temp;
		std::ostringstream sql;
		for (auto t = time_now; t <= time_after_year;)
		{
			time_temp.str("");
			time_temp.clear();
			time_temp << (std::localtime(&t)->tm_year + 1900)
				<< "-" << (std::localtime(&t)->tm_mon + 1)
				<< "-" << std::localtime(&t)->tm_mday
				<< " " << std::localtime(&t)->tm_hour
				<< ":" << std::localtime(&t)->tm_min
				<< ":" << std::localtime(&t)->tm_sec;

			for (int i = 0; i < 2; ++i) //2 lanes
			{
				sql.str("");
				sql.clear();
				sql << "insert into " << cm_.videoDetectDataTable_name
					<< "(" << CAMERA_NUMBER
					<< "," << CAMERA_RAW_LANE_NUMBER
					<< "," << CAMERA_UPLOAD_TIME
					<< "," << CAMERA_LANE_NUMBER
					<< "," << CAMERA_CAR_FLOW << ")"
					<< " values ("
					<< "'" << "thread7" << "'"
					<< "," << "'" << i + 1 << "'"
					<< "," << "to_date('" << time_temp.str() << "','YYYY-MM-DD HH24:MI:SS')"
					<< "," << "'" << lane_number[i] << "'";

				auto hour = std::localtime(&t)->tm_hour;
				if ((hour > 7 && hour < 10) || (hour > 17 && hour < 20)) //7-10 or 17-20 ,more cars
				{
					auto car_flow = std::rand() % cm_.max_peak_carFlow;
					sql << "," << "'" << car_flow << "'" << ")";
				}
				else
				{
					auto car_flow = std::rand() % cm_.max_low_carFlow;
					sql << "," << "'" << car_flow << "'" << ")";
				}
				dbWriter_ptr->addSql(sql.str());
			}
			t = t + 60; // each minute
			std::this_thread::sleep_for(std::chrono::milliseconds(sleep_rate));
		}
		printf("thread7 exit\n");
	});

	std::thread thread8([&time_now, &cm_, &message]()
	{
		auto dbWriter_ptr = std::make_shared<WwFoundation::DbWriter>(1, 500000, commit_size, message.str());
		std::vector<std::string> lane_number{ "515","516" };
		auto time_after_year = time_now + (cm_.days * 24 * 3600);
		std::ostringstream time_temp;
		std::ostringstream sql;
		for (auto t = time_now; t <= time_after_year;)
		{
			time_temp.str("");
			time_temp.clear();
			time_temp << (std::localtime(&t)->tm_year + 1900)
				<< "-" << (std::localtime(&t)->tm_mon + 1)
				<< "-" << std::localtime(&t)->tm_mday
				<< " " << std::localtime(&t)->tm_hour
				<< ":" << std::localtime(&t)->tm_min
				<< ":" << std::localtime(&t)->tm_sec;

			for (int i = 0; i < 2; ++i) //2 lanes
			{
				sql.str("");
				sql.clear();
				sql << "insert into " << cm_.videoDetectDataTable_name
					<< "(" << CAMERA_NUMBER
					<< "," << CAMERA_RAW_LANE_NUMBER
					<< "," << CAMERA_UPLOAD_TIME
					<< "," << CAMERA_LANE_NUMBER
					<< "," << CAMERA_CAR_FLOW << ")"
					<< " values ("
					<< "'" << "thread8" << "'"
					<< "," << "'" << i + 1 << "'"
					<< "," << "to_date('" << time_temp.str() << "','YYYY-MM-DD HH24:MI:SS')"
					<< "," << "'" << lane_number[i] << "'";

				auto hour = std::localtime(&t)->tm_hour;
				if ((hour > 7 && hour < 10) || (hour > 17 && hour < 20)) //7-10 or 17-20 ,more cars
				{
					auto car_flow = std::rand() % cm_.max_peak_carFlow;
					sql << "," << "'" << car_flow << "'" << ")";
				}
				else
				{
					auto car_flow = std::rand() % cm_.max_low_carFlow;
					sql << "," << "'" << car_flow << "'" << ")";
				}
				dbWriter_ptr->addSql(sql.str());
			}
			t = t + 60; // each minute
			std::this_thread::sleep_for(std::chrono::milliseconds(sleep_rate));
		}
		printf("thread8 exit\n");
	});

	std::thread thread9([&time_now, &cm_, &message]()
	{
		auto dbWriter_ptr = std::make_shared<WwFoundation::DbWriter>(1, 500000, commit_size, message.str());
		std::vector<std::string> lane_number{ "601","602" };
		auto time_after_year = time_now + (cm_.days * 24 * 3600);
		std::ostringstream time_temp;
		std::ostringstream sql;
		for (auto t = time_now; t <= time_after_year;)
		{
			time_temp.str("");
			time_temp.clear();
			time_temp << (std::localtime(&t)->tm_year + 1900)
				<< "-" << (std::localtime(&t)->tm_mon + 1)
				<< "-" << std::localtime(&t)->tm_mday
				<< " " << std::localtime(&t)->tm_hour
				<< ":" << std::localtime(&t)->tm_min
				<< ":" << std::localtime(&t)->tm_sec;

			for (int i = 0; i < 2; ++i) //2 lanes
			{
				sql.str("");
				sql.clear();
				sql << "insert into " << cm_.videoDetectDataTable_name
					<< "(" << CAMERA_NUMBER
					<< "," << CAMERA_RAW_LANE_NUMBER
					<< "," << CAMERA_UPLOAD_TIME
					<< "," << CAMERA_LANE_NUMBER
					<< "," << CAMERA_CAR_FLOW << ")"
					<< " values ("
					<< "'" << "thread9" << "'"
					<< "," << "'" << i + 1 << "'"
					<< "," << "to_date('" << time_temp.str() << "','YYYY-MM-DD HH24:MI:SS')"
					<< "," << "'" << lane_number[i] << "'";

				auto hour = std::localtime(&t)->tm_hour;
				if ((hour > 7 && hour < 10) || (hour > 17 && hour < 20)) //7-10 or 17-20 ,more cars
				{
					auto car_flow = std::rand() % cm_.max_peak_carFlow;
					sql << "," << "'" << car_flow << "'" << ")";
				}
				else
				{
					auto car_flow = std::rand() % cm_.max_low_carFlow;
					sql << "," << "'" << car_flow << "'" << ")";
				}
				dbWriter_ptr->addSql(sql.str());
			}
			t = t + 60; // each minute
			std::this_thread::sleep_for(std::chrono::milliseconds(sleep_rate));
		}
		printf("thread9 exit\n");
	});

	std::thread thread10([&time_now, &cm_, &message]()
	{
		auto dbWriter_ptr = std::make_shared<WwFoundation::DbWriter>(1, 500000, commit_size, message.str());
		std::vector<std::string> lane_number{ "603","604" };
		auto time_after_year = time_now + (cm_.days * 24 * 3600);
		std::ostringstream time_temp;
		std::ostringstream sql;
		for (auto t = time_now; t <= time_after_year;)
		{
			time_temp.str("");
			time_temp.clear();
			time_temp << (std::localtime(&t)->tm_year + 1900)
				<< "-" << (std::localtime(&t)->tm_mon + 1)
				<< "-" << std::localtime(&t)->tm_mday
				<< " " << std::localtime(&t)->tm_hour
				<< ":" << std::localtime(&t)->tm_min
				<< ":" << std::localtime(&t)->tm_sec;

			for (int i = 0; i < 2; ++i) //2 lanes
			{
				sql.str("");
				sql.clear();
				sql << "insert into " << cm_.videoDetectDataTable_name
					<< "(" << CAMERA_NUMBER
					<< "," << CAMERA_RAW_LANE_NUMBER
					<< "," << CAMERA_UPLOAD_TIME
					<< "," << CAMERA_LANE_NUMBER
					<< "," << CAMERA_CAR_FLOW << ")"
					<< " values ("
					<< "'" << "thread10" << "'"
					<< "," << "'" << i + 1 << "'"
					<< "," << "to_date('" << time_temp.str() << "','YYYY-MM-DD HH24:MI:SS')"
					<< "," << "'" << lane_number[i] << "'";

				auto hour = std::localtime(&t)->tm_hour;
				if ((hour > 7 && hour < 10) || (hour > 17 && hour < 20)) //7-10 or 17-20 ,more cars
				{
					auto car_flow = std::rand() % cm_.max_peak_carFlow;
					sql << "," << "'" << car_flow << "'" << ")";
				}
				else
				{
					auto car_flow = std::rand() % cm_.max_low_carFlow;
					sql << "," << "'" << car_flow << "'" << ")";
				}
				dbWriter_ptr->addSql(sql.str());
			}
			t = t + 60; // each minute
			std::this_thread::sleep_for(std::chrono::milliseconds(sleep_rate));
		}
		printf("thread10 exit\n");
	});

	std::thread thread11([&time_now, &cm_, &message]()
	{
		auto dbWriter_ptr = std::make_shared<WwFoundation::DbWriter>(1, 500000, commit_size, message.str());
		std::vector<std::string> lane_number{ "605","606" };
		auto time_after_year = time_now + (cm_.days * 24 * 3600);
		std::ostringstream time_temp;
		std::ostringstream sql;
		for (auto t = time_now; t <= time_after_year;)
		{
			time_temp.str("");
			time_temp.clear();
			time_temp << (std::localtime(&t)->tm_year + 1900)
				<< "-" << (std::localtime(&t)->tm_mon + 1)
				<< "-" << std::localtime(&t)->tm_mday
				<< " " << std::localtime(&t)->tm_hour
				<< ":" << std::localtime(&t)->tm_min
				<< ":" << std::localtime(&t)->tm_sec;

			for (int i = 0; i < 2; ++i) //2 lanes
			{
				sql.str("");
				sql.clear();
				sql << "insert into " << cm_.videoDetectDataTable_name
					<< "(" << CAMERA_NUMBER
					<< "," << CAMERA_RAW_LANE_NUMBER
					<< "," << CAMERA_UPLOAD_TIME
					<< "," << CAMERA_LANE_NUMBER
					<< "," << CAMERA_CAR_FLOW << ")"
					<< " values ("
					<< "'" << "thread11" << "'"
					<< "," << "'" << i + 1 << "'"
					<< "," << "to_date('" << time_temp.str() << "','YYYY-MM-DD HH24:MI:SS')"
					<< "," << "'" << lane_number[i] << "'";

				auto hour = std::localtime(&t)->tm_hour;
				if ((hour > 7 && hour < 10) || (hour > 17 && hour < 20)) //7-10 or 17-20 ,more cars
				{
					auto car_flow = std::rand() % cm_.max_peak_carFlow;
					sql << "," << "'" << car_flow << "'" << ")";
				}
				else
				{
					auto car_flow = std::rand() % cm_.max_low_carFlow;
					sql << "," << "'" << car_flow << "'" << ")";
				}
				dbWriter_ptr->addSql(sql.str());
			}
			t = t + 60; // each minute
			std::this_thread::sleep_for(std::chrono::milliseconds(sleep_rate));
		}
		printf("thread11 exit\n");
	});

	std::thread thread12([&time_now, &cm_, &message]()
	{
		auto dbWriter_ptr = std::make_shared<WwFoundation::DbWriter>(1, 500000, commit_size, message.str());
		std::vector<std::string> lane_number{ "607","608" };
		auto time_after_year = time_now + (cm_.days * 24 * 3600);
		std::ostringstream time_temp;
		std::ostringstream sql;
		for (auto t = time_now; t <= time_after_year;)
		{
			time_temp.str("");
			time_temp.clear();
			time_temp << (std::localtime(&t)->tm_year + 1900)
				<< "-" << (std::localtime(&t)->tm_mon + 1)
				<< "-" << std::localtime(&t)->tm_mday
				<< " " << std::localtime(&t)->tm_hour
				<< ":" << std::localtime(&t)->tm_min
				<< ":" << std::localtime(&t)->tm_sec;

			for (int i = 0; i < 2; ++i) //2 lanes
			{
				sql.str("");
				sql.clear();
				sql << "insert into " << cm_.videoDetectDataTable_name
					<< "(" << CAMERA_NUMBER
					<< "," << CAMERA_RAW_LANE_NUMBER
					<< "," << CAMERA_UPLOAD_TIME
					<< "," << CAMERA_LANE_NUMBER
					<< "," << CAMERA_CAR_FLOW << ")"
					<< " values ("
					<< "'" << "thread12" << "'"
					<< "," << "'" << i + 1 << "'"
					<< "," << "to_date('" << time_temp.str() << "','YYYY-MM-DD HH24:MI:SS')"
					<< "," << "'" << lane_number[i] << "'";

				auto hour = std::localtime(&t)->tm_hour;
				if ((hour > 7 && hour < 10) || (hour > 17 && hour < 20)) //7-10 or 17-20 ,more cars
				{
					auto car_flow = std::rand() % cm_.max_peak_carFlow;
					sql << "," << "'" << car_flow << "'" << ")";
				}
				else
				{
					auto car_flow = std::rand() % cm_.max_low_carFlow;
					sql << "," << "'" << car_flow << "'" << ")";
				}
				dbWriter_ptr->addSql(sql.str());
			}
			t = t + 60; // each minute
			std::this_thread::sleep_for(std::chrono::milliseconds(sleep_rate));
		}
		printf("thread12 exit\n");
	});

	std::thread thread13([&time_now, &cm_, &message]()
	{
		auto dbWriter_ptr = std::make_shared<WwFoundation::DbWriter>(1, 500000, commit_size, message.str());
		std::vector<std::string> lane_number{ "609","610" };
		auto time_after_year = time_now + (cm_.days * 24 * 3600);
		std::ostringstream time_temp;
		std::ostringstream sql;
		for (auto t = time_now; t <= time_after_year;)
		{
			time_temp.str("");
			time_temp.clear();
			time_temp << (std::localtime(&t)->tm_year + 1900)
				<< "-" << (std::localtime(&t)->tm_mon + 1)
				<< "-" << std::localtime(&t)->tm_mday
				<< " " << std::localtime(&t)->tm_hour
				<< ":" << std::localtime(&t)->tm_min
				<< ":" << std::localtime(&t)->tm_sec;

			for (int i = 0; i < 2; ++i) //2 lanes
			{
				sql.str("");
				sql.clear();
				sql << "insert into " << cm_.videoDetectDataTable_name
					<< "(" << CAMERA_NUMBER
					<< "," << CAMERA_RAW_LANE_NUMBER
					<< "," << CAMERA_UPLOAD_TIME
					<< "," << CAMERA_LANE_NUMBER
					<< "," << CAMERA_CAR_FLOW << ")"
					<< " values ("
					<< "'" << "thread13" << "'"
					<< "," << "'" << i + 1 << "'"
					<< "," << "to_date('" << time_temp.str() << "','YYYY-MM-DD HH24:MI:SS')"
					<< "," << "'" << lane_number[i] << "'";

				auto hour = std::localtime(&t)->tm_hour;
				if ((hour > 7 && hour < 10) || (hour > 17 && hour < 20)) //7-10 or 17-20 ,more cars
				{
					auto car_flow = std::rand() % cm_.max_peak_carFlow;
					sql << "," << "'" << car_flow << "'" << ")";
				}
				else
				{
					auto car_flow = std::rand() % cm_.max_low_carFlow;
					sql << "," << "'" << car_flow << "'" << ")";
				}
				dbWriter_ptr->addSql(sql.str());
			}
			t = t + 60; // each minute
			std::this_thread::sleep_for(std::chrono::milliseconds(sleep_rate));
		}
		printf("thread13 exit\n");
	});

	std::thread thread14([&time_now, &cm_, &message]()
	{
		auto dbWriter_ptr = std::make_shared<WwFoundation::DbWriter>(1, 500000, commit_size, message.str());
		std::vector<std::string> lane_number{ "611","612" };
		auto time_after_year = time_now + (cm_.days * 24 * 3600);
		std::ostringstream time_temp;
		std::ostringstream sql;
		for (auto t = time_now; t <= time_after_year;)
		{
			time_temp.str("");
			time_temp.clear();
			time_temp << (std::localtime(&t)->tm_year + 1900)
				<< "-" << (std::localtime(&t)->tm_mon + 1)
				<< "-" << std::localtime(&t)->tm_mday
				<< " " << std::localtime(&t)->tm_hour
				<< ":" << std::localtime(&t)->tm_min
				<< ":" << std::localtime(&t)->tm_sec;

			for (int i = 0; i < 2; ++i) //2 lanes
			{
				sql.str("");
				sql.clear();
				sql << "insert into " << cm_.videoDetectDataTable_name
					<< "(" << CAMERA_NUMBER
					<< "," << CAMERA_RAW_LANE_NUMBER
					<< "," << CAMERA_UPLOAD_TIME
					<< "," << CAMERA_LANE_NUMBER
					<< "," << CAMERA_CAR_FLOW << ")"
					<< " values ("
					<< "'" << "thread14" << "'"
					<< "," << "'" << i + 1 << "'"
					<< "," << "to_date('" << time_temp.str() << "','YYYY-MM-DD HH24:MI:SS')"
					<< "," << "'" << lane_number[i] << "'";

				auto hour = std::localtime(&t)->tm_hour;
				if ((hour > 7 && hour < 10) || (hour > 17 && hour < 20)) //7-10 or 17-20 ,more cars
				{
					auto car_flow = std::rand() % cm_.max_peak_carFlow;
					sql << "," << "'" << car_flow << "'" << ")";
				}
				else
				{
					auto car_flow = std::rand() % cm_.max_low_carFlow;
					sql << "," << "'" << car_flow << "'" << ")";
				}
				dbWriter_ptr->addSql(sql.str());
			}
			t = t + 60; // each minute
			std::this_thread::sleep_for(std::chrono::milliseconds(sleep_rate));
		}
		printf("thread14 exit\n");
	});

	std::thread thread15([&time_now, &cm_, &message]()
	{
		auto dbWriter_ptr = std::make_shared<WwFoundation::DbWriter>(1, 500000, commit_size, message.str());
		std::vector<std::string> lane_number{ "613","614" };
		auto time_after_year = time_now + (cm_.days * 24 * 3600);
		std::ostringstream time_temp;
		std::ostringstream sql;
		for (auto t = time_now; t <= time_after_year;)
		{
			time_temp.str("");
			time_temp.clear();
			time_temp << (std::localtime(&t)->tm_year + 1900)
				<< "-" << (std::localtime(&t)->tm_mon + 1)
				<< "-" << std::localtime(&t)->tm_mday
				<< " " << std::localtime(&t)->tm_hour
				<< ":" << std::localtime(&t)->tm_min
				<< ":" << std::localtime(&t)->tm_sec;

			for (int i = 0; i < 2; ++i) //2 lanes
			{
				sql.str("");
				sql.clear();
				sql << "insert into " << cm_.videoDetectDataTable_name
					<< "(" << CAMERA_NUMBER
					<< "," << CAMERA_RAW_LANE_NUMBER
					<< "," << CAMERA_UPLOAD_TIME
					<< "," << CAMERA_LANE_NUMBER
					<< "," << CAMERA_CAR_FLOW << ")"
					<< " values ("
					<< "'" << "thread15" << "'"
					<< "," << "'" << i + 1 << "'"
					<< "," << "to_date('" << time_temp.str() << "','YYYY-MM-DD HH24:MI:SS')"
					<< "," << "'" << lane_number[i] << "'";

				auto hour = std::localtime(&t)->tm_hour;
				if ((hour > 7 && hour < 10) || (hour > 17 && hour < 20)) //7-10 or 17-20 ,more cars
				{
					auto car_flow = std::rand() % cm_.max_peak_carFlow;
					sql << "," << "'" << car_flow << "'" << ")";
				}
				else
				{
					auto car_flow = std::rand() % cm_.max_low_carFlow;
					sql << "," << "'" << car_flow << "'" << ")";
				}
				dbWriter_ptr->addSql(sql.str());
			}
			t = t + 60; // each minute
			std::this_thread::sleep_for(std::chrono::milliseconds(sleep_rate));
		}
		printf("thread15 exit\n");
	});

	std::thread thread16([&time_now, &cm_, &message]()
	{
		auto dbWriter_ptr = std::make_shared<WwFoundation::DbWriter>(1, 500000, commit_size, message.str());
		std::vector<std::string> lane_number{ "615","616" };
		auto time_after_year = time_now + (cm_.days * 24 * 3600);
		std::ostringstream time_temp;
		std::ostringstream sql;
		for (auto t = time_now; t <= time_after_year;)
		{
			time_temp.str("");
			time_temp.clear();
			time_temp << (std::localtime(&t)->tm_year + 1900)
				<< "-" << (std::localtime(&t)->tm_mon + 1)
				<< "-" << std::localtime(&t)->tm_mday
				<< " " << std::localtime(&t)->tm_hour
				<< ":" << std::localtime(&t)->tm_min
				<< ":" << std::localtime(&t)->tm_sec;

			for (int i = 0; i < 2; ++i) //2 lanes
			{
				sql.str("");
				sql.clear();
				sql << "insert into " << cm_.videoDetectDataTable_name
					<< "(" << CAMERA_NUMBER
					<< "," << CAMERA_RAW_LANE_NUMBER
					<< "," << CAMERA_UPLOAD_TIME
					<< "," << CAMERA_LANE_NUMBER
					<< "," << CAMERA_CAR_FLOW << ")"
					<< " values ("
					<< "'" << "thread16" << "'"
					<< "," << "'" << i + 1 << "'"
					<< "," << "to_date('" << time_temp.str() << "','YYYY-MM-DD HH24:MI:SS')"
					<< "," << "'" << lane_number[i] << "'";

				auto hour = std::localtime(&t)->tm_hour;
				if ((hour > 7 && hour < 10) || (hour > 17 && hour < 20)) //7-10 or 17-20 ,more cars
				{
					auto car_flow = std::rand() % cm_.max_peak_carFlow;
					sql << "," << "'" << car_flow << "'" << ")";
				}
				else
				{
					auto car_flow = std::rand() % cm_.max_low_carFlow;
					sql << "," << "'" << car_flow << "'" << ")";
				}
				dbWriter_ptr->addSql(sql.str());
			}
			t = t + 60; // each minute
			std::this_thread::sleep_for(std::chrono::milliseconds(sleep_rate));
		}
		printf("thread16 exit\n");
	});

	thread1.join();
	thread2.join();
	thread3.join();
	thread4.join();
	thread5.join();
	thread6.join();
	thread7.join();
	thread8.join();
	thread9.join();
	thread10.join();
	thread11.join();
	thread12.join();
	thread13.join();
	thread14.join();
	thread15.join();
	thread16.join();
	system("pause");
	return 0;
}