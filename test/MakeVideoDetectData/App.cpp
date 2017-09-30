#include <ctime>
#include <string>
#include <sstream>
#include <memory>
#include <thread>
#pragma warning(disable: 4996)
#include "DbWriter.h"
#include "ParseIni.h"

// 2 crossing , 16 cameras, 32 lanes ,so 16 threads to write data
const std::string CAMERA_NUMBER = "camera_number";
const std::string CAMERA_LANE_NUMBER = "lane_number";
const std::string CAMERA_CAR_FLOW = "car_flow";
const std::string CAMERA_UPLOAD_TIME = "upload_time";
const std::string CAMERA_RAW_LANE_NUMBER = "raw_lane_number";
const int sleep_rate = 10;//ms
const int commit_size = 1000;

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
	printf("make %d days data\n", cm_.days);
	auto time_now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()); //now time(second)
	std::ostringstream message;
	message << cm_.db_UserName << "/" << cm_.db_password << "@" <<
		cm_.db_ip << ":" << cm_.db_port << "/" << cm_.db_name;

	std::thread thread1([&time_now, &cm_, &message]()
	{
		auto dbWriter_ptr = std::make_shared<WwFoundation::DbWriter>(1, 500000, commit_size, message.str());
		std::vector<std::string> lane_number{ "502", "501" };
		auto time_before = time_now -(cm_.days * 24 * 3600);
		printf("thread1 make date for camera(0ce72c14f0f2456d824f34125f0c7b48),time_before:%lld\n", time_before);
		std::ostringstream time_temp;
		std::ostringstream sql;
		for (auto t = time_before; t <=time_now;)
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
					<< "'" << "0ce72c14f0f2456d824f34125f0c7b48" << "'"
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
		dbWriter_ptr->stop();
	});

	std::thread thread2([&time_now, &cm_, &message]()
	{
		auto dbWriter_ptr = std::make_shared<WwFoundation::DbWriter>(1, 500000, commit_size, message.str());
		std::vector<std::string> lane_number{ "504","503" };
		auto time_before = time_now - (cm_.days * 24 * 3600);
		printf("thread2 make date for camera(fd0fe65f45974e079d1a35bcb56bd755),time_before:%lld\n", time_before);
		std::ostringstream time_temp;
		std::ostringstream sql;
		for (auto t = time_before; t <= time_now;)
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
					<< "'" << "fd0fe65f45974e079d1a35bcb56bd755" << "'"
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
		dbWriter_ptr->stop();
	});

	std::thread thread3([&time_now, &cm_, &message]()
	{
		auto dbWriter_ptr = std::make_shared<WwFoundation::DbWriter>(1, 500000, commit_size, message.str());
		std::vector<std::string> lane_number{ "506","505" };
		auto time_before = time_now - (cm_.days * 24 * 3600);
		printf("thread3 make date for camera(1cba377bfbed46d58b07f0155b01ea31),time_before:%lld\n", time_before);
		std::ostringstream time_temp;
		std::ostringstream sql;
		for (auto t = time_before; t <= time_now;)
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
					<< "'" << "1cba377bfbed46d58b07f0155b01ea31" << "'"
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
		dbWriter_ptr->stop();
	});

	std::thread thread4([&time_now, &cm_, &message]()
	{
		auto dbWriter_ptr = std::make_shared<WwFoundation::DbWriter>(1, 500000, commit_size, message.str());
		std::vector<std::string> lane_number{ "508","507" };
		auto time_before = time_now - (cm_.days * 24 * 3600);
		printf("thread4 make date for camera(57fcd01be29e4545b77c9ced905a9d2a),time_before:%lld\n", time_before);
		std::ostringstream time_temp;
		std::ostringstream sql;
		for (auto t = time_before; t <= time_now;)
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
					<< "'" << "57fcd01be29e4545b77c9ced905a9d2a" << "'"
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
		dbWriter_ptr->stop();
	});

	std::thread thread5([&time_now, &cm_, &message]()
	{
		auto dbWriter_ptr = std::make_shared<WwFoundation::DbWriter>(1, 500000, commit_size, message.str());
		std::vector<std::string> lane_number{ "512","511" };
		auto time_before = time_now - (cm_.days * 24 * 3600);
		printf("thread5 make date for camera(11a6427b02a048068cf26cfb675beabe),time_before:%lld\n", time_before);
		std::ostringstream time_temp;
		std::ostringstream sql;
		for (auto t = time_before; t <= time_now;)
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
					<< "'" << "11a6427b02a048068cf26cfb675beabe" << "'"
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
		dbWriter_ptr->stop();
	});

	std::thread thread6([&time_now, &cm_, &message]()
	{
		auto dbWriter_ptr = std::make_shared<WwFoundation::DbWriter>(1, 500000, commit_size, message.str());
		std::vector<std::string> lane_number{ "515","514","513" };
		auto time_before = time_now -(cm_.days * 24 * 3600);
		printf("thread6 make date for camera(6ab80463a8214fe499dc04bc65d0f29a),time_before:%lld\n", time_before);
		std::ostringstream time_temp;
		std::ostringstream sql;
		for (auto t = time_before; t <= time_now;)
		{
			time_temp.str("");
			time_temp.clear();
			time_temp << (std::localtime(&t)->tm_year + 1900)
				<< "-" << (std::localtime(&t)->tm_mon + 1)
				<< "-" << std::localtime(&t)->tm_mday
				<< " " << std::localtime(&t)->tm_hour
				<< ":" << std::localtime(&t)->tm_min
				<< ":" << std::localtime(&t)->tm_sec;
			for (int i = 0; i < 3; ++i) //3 lanes
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
					<< "'" << "6ab80463a8214fe499dc04bc65d0f29a" << "'"
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
		dbWriter_ptr->stop();
	});

	std::thread thread7([&time_now, &cm_, &message]()
	{
		auto dbWriter_ptr = std::make_shared<WwFoundation::DbWriter>(1, 500000, commit_size, message.str());
		std::vector<std::string> lane_number{ "502","501" };
		auto time_before = time_now - (cm_.days * 24 * 3600);
		printf("thread7 make date for camera(fefb52f858de4447848aa1b7c517bd8f),time_before:%lld\n", time_before);
		std::ostringstream time_temp;
		std::ostringstream sql;
		for (auto t = time_before; t <= time_now;)
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
					<< "'" << "fefb52f858de4447848aa1b7c517bd8f" << "'"
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
		dbWriter_ptr->stop();
	});

	std::thread thread8([&time_now, &cm_, &message]()
	{
		auto dbWriter_ptr = std::make_shared<WwFoundation::DbWriter>(1, 500000, commit_size, message.str());
		std::vector<std::string> lane_number{ "505","504","503" };
		auto time_before = time_now - (cm_.days * 24 * 3600);
		printf("thread8 make date for camera(e92a1213d3b64767b0e424b01463d0f1),time_before:%lld\n", time_before);
		std::ostringstream time_temp;
		std::ostringstream sql;
		for (auto t = time_before; t <= time_now;)
		{
			time_temp.str("");
			time_temp.clear();
			time_temp << (std::localtime(&t)->tm_year + 1900)
				<< "-" << (std::localtime(&t)->tm_mon + 1)
				<< "-" << std::localtime(&t)->tm_mday
				<< " " << std::localtime(&t)->tm_hour
				<< ":" << std::localtime(&t)->tm_min
				<< ":" << std::localtime(&t)->tm_sec;
			for (int i = 0; i < 3; ++i) //3 lanes
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
					<< "'" << "e92a1213d3b64767b0e424b01463d0f1" << "'"
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
		dbWriter_ptr->stop();
	});

	std::thread thread9([&time_now, &cm_, &message]()
	{
		auto dbWriter_ptr = std::make_shared<WwFoundation::DbWriter>(1, 500000, commit_size, message.str());
		std::vector<std::string> lane_number{ "508","507","506" };
		auto time_before = time_now -(cm_.days * 24 * 3600);
		printf("thread9 make date for camera(a39db5dd3ef246189745bf3954fbcdf5),time_before:%lld\n", time_before);
		std::ostringstream time_temp;
		std::ostringstream sql;
		for (auto t = time_before; t <= time_now;)
		{
			time_temp.str("");
			time_temp.clear();
			time_temp << (std::localtime(&t)->tm_year + 1900)
				<< "-" << (std::localtime(&t)->tm_mon + 1)
				<< "-" << std::localtime(&t)->tm_mday
				<< " " << std::localtime(&t)->tm_hour
				<< ":" << std::localtime(&t)->tm_min
				<< ":" << std::localtime(&t)->tm_sec;
			for (int i = 0; i < 3; ++i) //3 lanes
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
					<< "'" << "a39db5dd3ef246189745bf3954fbcdf5" << "'"
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
		dbWriter_ptr->stop();
	});

	std::thread thread10([&time_now, &cm_, &message]()
	{
		auto dbWriter_ptr = std::make_shared<WwFoundation::DbWriter>(1, 500000, commit_size, message.str());
		std::vector<std::string> lane_number{ "511","510","509" };
		auto time_before = time_now - (cm_.days * 24 * 3600);
		printf("thread10 make date for camera(1e7fcf8e674c449c9299adad66e400a1),time_before:%lld\n", time_before);
		std::ostringstream time_temp;
		std::ostringstream sql;
		for (auto t = time_before; t <= time_now;)
		{
			time_temp.str("");
			time_temp.clear();
			time_temp << (std::localtime(&t)->tm_year + 1900)
				<< "-" << (std::localtime(&t)->tm_mon + 1)
				<< "-" << std::localtime(&t)->tm_mday
				<< " " << std::localtime(&t)->tm_hour
				<< ":" << std::localtime(&t)->tm_min
				<< ":" << std::localtime(&t)->tm_sec;
			for (int i = 0; i < 3; ++i) //3 lanes
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
					<< "'" << "1e7fcf8e674c449c9299adad66e400a1" << "'"
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
		dbWriter_ptr->stop();
	});











	//std::thread thread11([&time_now, &cm_, &message]()
	//{
	//	auto dbWriter_ptr = std::make_shared<WwFoundation::DbWriter>(1, 500000, commit_size, message.str());
	//	std::vector<std::string> lane_number{ "605","606" };
	//	auto time_before = time_now + (cm_.days * 24 * 3600);
	//	std::ostringstream time_temp;
	//	std::ostringstream sql;
	//	for (auto t = time_before; t <= time_now;)
	//	{
	//		time_temp.str("");
	//		time_temp.clear();
	//		time_temp << (std::localtime(&t)->tm_year + 1900)
	//			<< "-" << (std::localtime(&t)->tm_mon + 1)
	//			<< "-" << std::localtime(&t)->tm_mday
	//			<< " " << std::localtime(&t)->tm_hour
	//			<< ":" << std::localtime(&t)->tm_min
	//			<< ":" << std::localtime(&t)->tm_sec;

	//		for (int i = 0; i < 2; ++i) //2 lanes
	//		{
	//			sql.str("");
	//			sql.clear();
	//			sql << "insert into " << cm_.videoDetectDataTable_name
	//				<< "(" << CAMERA_NUMBER
	//				<< "," << CAMERA_RAW_LANE_NUMBER
	//				<< "," << CAMERA_UPLOAD_TIME
	//				<< "," << CAMERA_LANE_NUMBER
	//				<< "," << CAMERA_CAR_FLOW << ")"
	//				<< " values ("
	//				<< "'" << "thread11" << "'"
	//				<< "," << "'" << i + 1 << "'"
	//				<< "," << "to_date('" << time_temp.str() << "','YYYY-MM-DD HH24:MI:SS')"
	//				<< "," << "'" << lane_number[i] << "'";

	//			auto hour = std::localtime(&t)->tm_hour;
	//			if ((hour > 7 && hour < 10) || (hour > 17 && hour < 20)) //7-10 or 17-20 ,more cars
	//			{
	//				auto car_flow = std::rand() % cm_.max_peak_carFlow;
	//				sql << "," << "'" << car_flow << "'" << ")";
	//			}
	//			else
	//			{
	//				auto car_flow = std::rand() % cm_.max_low_carFlow;
	//				sql << "," << "'" << car_flow << "'" << ")";
	//			}
	//			dbWriter_ptr->addSql(sql.str());
	//		}
	//		t = t + 60; // each minute
	//		std::this_thread::sleep_for(std::chrono::milliseconds(sleep_rate));
	//	}
	//	printf("thread11 exit\n");
	//});

	//std::thread thread12([&time_now, &cm_, &message]()
	//{
	//	auto dbWriter_ptr = std::make_shared<WwFoundation::DbWriter>(1, 500000, commit_size, message.str());
	//	std::vector<std::string> lane_number{ "607","608" };
	//	auto time_before = time_now + (cm_.days * 24 * 3600);
	//	std::ostringstream time_temp;
	//	std::ostringstream sql;
	//	for (auto t = time_before; t <= time_now;)
	//	{
	//		time_temp.str("");
	//		time_temp.clear();
	//		time_temp << (std::localtime(&t)->tm_year + 1900)
	//			<< "-" << (std::localtime(&t)->tm_mon + 1)
	//			<< "-" << std::localtime(&t)->tm_mday
	//			<< " " << std::localtime(&t)->tm_hour
	//			<< ":" << std::localtime(&t)->tm_min
	//			<< ":" << std::localtime(&t)->tm_sec;

	//		for (int i = 0; i < 2; ++i) //2 lanes
	//		{
	//			sql.str("");
	//			sql.clear();
	//			sql << "insert into " << cm_.videoDetectDataTable_name
	//				<< "(" << CAMERA_NUMBER
	//				<< "," << CAMERA_RAW_LANE_NUMBER
	//				<< "," << CAMERA_UPLOAD_TIME
	//				<< "," << CAMERA_LANE_NUMBER
	//				<< "," << CAMERA_CAR_FLOW << ")"
	//				<< " values ("
	//				<< "'" << "thread12" << "'"
	//				<< "," << "'" << i + 1 << "'"
	//				<< "," << "to_date('" << time_temp.str() << "','YYYY-MM-DD HH24:MI:SS')"
	//				<< "," << "'" << lane_number[i] << "'";

	//			auto hour = std::localtime(&t)->tm_hour;
	//			if ((hour > 7 && hour < 10) || (hour > 17 && hour < 20)) //7-10 or 17-20 ,more cars
	//			{
	//				auto car_flow = std::rand() % cm_.max_peak_carFlow;
	//				sql << "," << "'" << car_flow << "'" << ")";
	//			}
	//			else
	//			{
	//				auto car_flow = std::rand() % cm_.max_low_carFlow;
	//				sql << "," << "'" << car_flow << "'" << ")";
	//			}
	//			dbWriter_ptr->addSql(sql.str());
	//		}
	//		t = t + 60; // each minute
	//		std::this_thread::sleep_for(std::chrono::milliseconds(sleep_rate));
	//	}
	//	printf("thread12 exit\n");
	//});

	//std::thread thread13([&time_now, &cm_, &message]()
	//{
	//	auto dbWriter_ptr = std::make_shared<WwFoundation::DbWriter>(1, 500000, commit_size, message.str());
	//	std::vector<std::string> lane_number{ "609","610" };
	//	auto time_before = time_now + (cm_.days * 24 * 3600);
	//	std::ostringstream time_temp;
	//	std::ostringstream sql;
	//	for (auto t = time_before; t <= time_now;)
	//	{
	//		time_temp.str("");
	//		time_temp.clear();
	//		time_temp << (std::localtime(&t)->tm_year + 1900)
	//			<< "-" << (std::localtime(&t)->tm_mon + 1)
	//			<< "-" << std::localtime(&t)->tm_mday
	//			<< " " << std::localtime(&t)->tm_hour
	//			<< ":" << std::localtime(&t)->tm_min
	//			<< ":" << std::localtime(&t)->tm_sec;

	//		for (int i = 0; i < 2; ++i) //2 lanes
	//		{
	//			sql.str("");
	//			sql.clear();
	//			sql << "insert into " << cm_.videoDetectDataTable_name
	//				<< "(" << CAMERA_NUMBER
	//				<< "," << CAMERA_RAW_LANE_NUMBER
	//				<< "," << CAMERA_UPLOAD_TIME
	//				<< "," << CAMERA_LANE_NUMBER
	//				<< "," << CAMERA_CAR_FLOW << ")"
	//				<< " values ("
	//				<< "'" << "thread13" << "'"
	//				<< "," << "'" << i + 1 << "'"
	//				<< "," << "to_date('" << time_temp.str() << "','YYYY-MM-DD HH24:MI:SS')"
	//				<< "," << "'" << lane_number[i] << "'";

	//			auto hour = std::localtime(&t)->tm_hour;
	//			if ((hour > 7 && hour < 10) || (hour > 17 && hour < 20)) //7-10 or 17-20 ,more cars
	//			{
	//				auto car_flow = std::rand() % cm_.max_peak_carFlow;
	//				sql << "," << "'" << car_flow << "'" << ")";
	//			}
	//			else
	//			{
	//				auto car_flow = std::rand() % cm_.max_low_carFlow;
	//				sql << "," << "'" << car_flow << "'" << ")";
	//			}
	//			dbWriter_ptr->addSql(sql.str());
	//		}
	//		t = t + 60; // each minute
	//		std::this_thread::sleep_for(std::chrono::milliseconds(sleep_rate));
	//	}
	//	printf("thread13 exit\n");
	//});

	//std::thread thread14([&time_now, &cm_, &message]()
	//{
	//	auto dbWriter_ptr = std::make_shared<WwFoundation::DbWriter>(1, 500000, commit_size, message.str());
	//	std::vector<std::string> lane_number{ "611","612" };
	//	auto time_before = time_now + (cm_.days * 24 * 3600);
	//	std::ostringstream time_temp;
	//	std::ostringstream sql;
	//	for (auto t = time_before; t <= time_now;)
	//	{
	//		time_temp.str("");
	//		time_temp.clear();
	//		time_temp << (std::localtime(&t)->tm_year + 1900)
	//			<< "-" << (std::localtime(&t)->tm_mon + 1)
	//			<< "-" << std::localtime(&t)->tm_mday
	//			<< " " << std::localtime(&t)->tm_hour
	//			<< ":" << std::localtime(&t)->tm_min
	//			<< ":" << std::localtime(&t)->tm_sec;

	//		for (int i = 0; i < 2; ++i) //2 lanes
	//		{
	//			sql.str("");
	//			sql.clear();
	//			sql << "insert into " << cm_.videoDetectDataTable_name
	//				<< "(" << CAMERA_NUMBER
	//				<< "," << CAMERA_RAW_LANE_NUMBER
	//				<< "," << CAMERA_UPLOAD_TIME
	//				<< "," << CAMERA_LANE_NUMBER
	//				<< "," << CAMERA_CAR_FLOW << ")"
	//				<< " values ("
	//				<< "'" << "thread14" << "'"
	//				<< "," << "'" << i + 1 << "'"
	//				<< "," << "to_date('" << time_temp.str() << "','YYYY-MM-DD HH24:MI:SS')"
	//				<< "," << "'" << lane_number[i] << "'";

	//			auto hour = std::localtime(&t)->tm_hour;
	//			if ((hour > 7 && hour < 10) || (hour > 17 && hour < 20)) //7-10 or 17-20 ,more cars
	//			{
	//				auto car_flow = std::rand() % cm_.max_peak_carFlow;
	//				sql << "," << "'" << car_flow << "'" << ")";
	//			}
	//			else
	//			{
	//				auto car_flow = std::rand() % cm_.max_low_carFlow;
	//				sql << "," << "'" << car_flow << "'" << ")";
	//			}
	//			dbWriter_ptr->addSql(sql.str());
	//		}
	//		t = t + 60; // each minute
	//		std::this_thread::sleep_for(std::chrono::milliseconds(sleep_rate));
	//	}
	//	printf("thread14 exit\n");
	//});

	//std::thread thread15([&time_now, &cm_, &message]()
	//{
	//	auto dbWriter_ptr = std::make_shared<WwFoundation::DbWriter>(1, 500000, commit_size, message.str());
	//	std::vector<std::string> lane_number{ "613","614" };
	//	auto time_before = time_now + (cm_.days * 24 * 3600);
	//	std::ostringstream time_temp;
	//	std::ostringstream sql;
	//	for (auto t = time_before; t <= time_now;)
	//	{
	//		time_temp.str("");
	//		time_temp.clear();
	//		time_temp << (std::localtime(&t)->tm_year + 1900)
	//			<< "-" << (std::localtime(&t)->tm_mon + 1)
	//			<< "-" << std::localtime(&t)->tm_mday
	//			<< " " << std::localtime(&t)->tm_hour
	//			<< ":" << std::localtime(&t)->tm_min
	//			<< ":" << std::localtime(&t)->tm_sec;

	//		for (int i = 0; i < 2; ++i) //2 lanes
	//		{
	//			sql.str("");
	//			sql.clear();
	//			sql << "insert into " << cm_.videoDetectDataTable_name
	//				<< "(" << CAMERA_NUMBER
	//				<< "," << CAMERA_RAW_LANE_NUMBER
	//				<< "," << CAMERA_UPLOAD_TIME
	//				<< "," << CAMERA_LANE_NUMBER
	//				<< "," << CAMERA_CAR_FLOW << ")"
	//				<< " values ("
	//				<< "'" << "thread15" << "'"
	//				<< "," << "'" << i + 1 << "'"
	//				<< "," << "to_date('" << time_temp.str() << "','YYYY-MM-DD HH24:MI:SS')"
	//				<< "," << "'" << lane_number[i] << "'";

	//			auto hour = std::localtime(&t)->tm_hour;
	//			if ((hour > 7 && hour < 10) || (hour > 17 && hour < 20)) //7-10 or 17-20 ,more cars
	//			{
	//				auto car_flow = std::rand() % cm_.max_peak_carFlow;
	//				sql << "," << "'" << car_flow << "'" << ")";
	//			}
	//			else
	//			{
	//				auto car_flow = std::rand() % cm_.max_low_carFlow;
	//				sql << "," << "'" << car_flow << "'" << ")";
	//			}
	//			dbWriter_ptr->addSql(sql.str());
	//		}
	//		t = t + 60; // each minute
	//		std::this_thread::sleep_for(std::chrono::milliseconds(sleep_rate));
	//	}
	//	printf("thread15 exit\n");
	//});

	//std::thread thread16([&time_now, &cm_, &message]()
	//{
	//	auto dbWriter_ptr = std::make_shared<WwFoundation::DbWriter>(1, 500000, commit_size, message.str());
	//	std::vector<std::string> lane_number{ "615","616" };
	//	auto time_before = time_now + (cm_.days * 24 * 3600);
	//	std::ostringstream time_temp;
	//	std::ostringstream sql;
	//	for (auto t = time_before; t <= time_now;)
	//	{
	//		time_temp.str("");
	//		time_temp.clear();
	//		time_temp << (std::localtime(&t)->tm_year + 1900)
	//			<< "-" << (std::localtime(&t)->tm_mon + 1)
	//			<< "-" << std::localtime(&t)->tm_mday
	//			<< " " << std::localtime(&t)->tm_hour
	//			<< ":" << std::localtime(&t)->tm_min
	//			<< ":" << std::localtime(&t)->tm_sec;

	//		for (int i = 0; i < 2; ++i) //2 lanes
	//		{
	//			sql.str("");
	//			sql.clear();
	//			sql << "insert into " << cm_.videoDetectDataTable_name
	//				<< "(" << CAMERA_NUMBER
	//				<< "," << CAMERA_RAW_LANE_NUMBER
	//				<< "," << CAMERA_UPLOAD_TIME
	//				<< "," << CAMERA_LANE_NUMBER
	//				<< "," << CAMERA_CAR_FLOW << ")"
	//				<< " values ("
	//				<< "'" << "thread16" << "'"
	//				<< "," << "'" << i + 1 << "'"
	//				<< "," << "to_date('" << time_temp.str() << "','YYYY-MM-DD HH24:MI:SS')"
	//				<< "," << "'" << lane_number[i] << "'";

	//			auto hour = std::localtime(&t)->tm_hour;
	//			if ((hour > 7 && hour < 10) || (hour > 17 && hour < 20)) //7-10 or 17-20 ,more cars
	//			{
	//				auto car_flow = std::rand() % cm_.max_peak_carFlow;
	//				sql << "," << "'" << car_flow << "'" << ")";
	//			}
	//			else
	//			{
	//				auto car_flow = std::rand() % cm_.max_low_carFlow;
	//				sql << "," << "'" << car_flow << "'" << ")";
	//			}
	//			dbWriter_ptr->addSql(sql.str());
	//		}
	//		t = t + 60; // each minute
	//		std::this_thread::sleep_for(std::chrono::milliseconds(sleep_rate));
	//	}
	//	printf("thread16 exit\n");
	//});

	if (thread1.joinable())
		thread1.join();
	if (thread2.joinable())
		thread2.join();
	if (thread3.joinable())
		thread3.join();
	if (thread4.joinable())
		thread4.join();
	if (thread5.joinable())
		thread5.join();
	if (thread6.joinable())
		thread6.join();
	if (thread7.joinable())
		thread7.join();
	if (thread8.joinable())
		thread8.join();
	if (thread9.joinable())
		thread9.join();
	if (thread10.joinable())
		thread10.join();


	/*thread11.join();
	thread12.join();
	thread13.join();
	thread14.join();
	thread15.join();
	thread16.join();*/
	system("pause");
	return 0;
}