#define OTL_ORA12C_R2 // Compile OTL 4.0/OCI12
#define OTL_STL // Enable STL compatibility mode
//#define OTL_UNICODE

#include "otlv4.h" // include the OTL 4.0 header file
#include <vector>


//void insert()
//// insert rows into table
//{
//	std::string f2;
//	otl_stream o(3,
//		"insert into test_tab values(:f1<int>,:f2<char[4001]>)",
//		// SQL statement
//		db // connect object
//	);
//
//	std::string json_str(json_sample);
//
//	o << 1 << json_str;
//	o << 2 << json_str;
//
//}

//void select()
//{
//	std::string f2;
//
//	otl_stream i(3,
//		"select * from test_tab where f1>=:f<int> and f1<=:f*2",
//		// SELECT statement
//		db // connect object
//	);
//	// create select stream
//
//	int f1;
//
//	i << 1; // assigning :f = 1
//			// SELECT automatically executes when all input variables are
//			// assigned. First portion of output rows is fetched to the buffer
//
//	while (!i.eof()) { // while not end-of-data
//		i >> f1 >> f2;
//		std::cout << "f1=" << f1 << ", f2=" << f2 << std::endl;
//		std::cout << "======================================================" << std::endl;
//	}
//
//}

int main()
{
	otl_connect db; // connect object
	otl_connect::otl_initialize(); // initialize OCI environment
	try
	{
		db.rlogon("wangwei/111111@192.168.1.192:1521/abcdef"); // connect to Oracle
		
		std::string sql = "select devicemodel,DEVICEID,DEVICEIP from device where devicemodel in"
			"('FusionServer 2288V3','H&i Server 2101','RG-WALL 1600-M6600','RG-IS2706G')";
		otl_stream i(1,
			sql.c_str(),// SELECT statement
			db // connect object
		);
		std::string device_id = "";
		std::string	device_ip = "";
		std::string	device_model = "";
		while (!i.eof())
		{
			i >> device_model >> device_ip >> device_id;
			std::cout << "device_model=" << device_model
				<< "  device_ip=" << device_ip
				<< "  device_id=" << device_id <<std::endl;
		}
	}
	
	catch (otl_exception& p)
	{ // intercept OTL exceptions
		std::cerr << p.msg << std::endl; // print out error message
		std::cerr << p.stm_text << std::endl; // print out SQL that caused the error
		std::cerr << p.var_info << std::endl; // print out the variable that caused the error
	}
	db.logoff(); // disconnect from Oracle
	system("pause");
	return 0;
}