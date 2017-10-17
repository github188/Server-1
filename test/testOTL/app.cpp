#define OTL_ORA12C_R2 // Compile OTL 4.0/OCI12
#define OTL_STL // Enable STL compatibility mode
#define OTL_UNICODE
#include "windows.h"
#include "otlv4.h" // include the OTL 4.0 header file
#include <vector>
#include <iostream>
#include <codecvt>
#include <locale>


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


std::string GBKToUTF8(const std::string& strGBK)
{
	std::string strOutUTF8 = "";
	WCHAR * str1;
	int n = MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, NULL, 0);
	str1 = new WCHAR[n];
	MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, str1, n);
	n = WideCharToMultiByte(CP_UTF8, 0, str1, -1, NULL, 0, NULL, NULL);
	char * str2 = new char[n];
	WideCharToMultiByte(CP_UTF8, 0, str1, -1, str2, n, NULL, NULL);
	strOutUTF8 = str2;
	delete[]str1;
	str1 = NULL;
	delete[]str2;
	str2 = NULL;
	return strOutUTF8;
}

std::string UtfToGbk(const char* utf8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wstr, len);
	len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);
	if (wstr) delete[] wstr;
	return str;
}

std::string gb2312_to_utf8(std::string const &strGb2312)
{
	std::vector<wchar_t> buff(strGb2312.size());
#ifdef _MSC_VER
	std::locale loc("zh-CN");
#else
	std::locale loc("zh_CN.GB18030");
#endif
	wchar_t* pwszNext = nullptr;
	const char* pszNext = nullptr;
	mbstate_t state = {};
	int res = std::use_facet<std::codecvt<wchar_t, char, mbstate_t> >
		(loc).in(state,
			strGb2312.data(), strGb2312.data() + strGb2312.size(), pszNext,
			buff.data(), buff.data() + buff.size(), pwszNext);
	if (std::codecvt_base::ok == res)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> cutf8;
		return cutf8.to_bytes(std::wstring(buff.data(), pwszNext));
	}
	return "";
}

int main()
{	
	SetEnvironmentVariable(L"NLS_LANG", L"SIMPLIFIED CHINESE_CHINA.AL32UTF8");


	otl_connect db; // connect object
	otl_connect::otl_initialize(); // initialize OCI environment
	try
	{
		//db.rlogon("wangwei/111111@192.168.1.187:1521/arcgis"); // connect to Oracle
		//db.rlogon("wangwei/111111@192.168.1.192:1521/abcdef"); // connect to Oracle
		//std::string sql = "select model,u_number,IP from its_device where IP='192.168.1.20'";
		//std::string sql = "update its_device set model='°²' where IP='192.168.1.20'";

		db.rlogon("sde/gmi12345@192.168.1.187:1521/arcgis"); // connect to Oracle
		std::string brand = "½Ü";
		auto brand_str = brand.c_str();
		auto utf8_brand = GBKToUTF8(brand);
		auto utf8_brand_str = utf8_brand.c_str();
		std::string sql = "update its_device set brand='";
		std::string sql1 = "' where type='57'";
		sql.append(utf8_brand).append(sql1);
		otl_stream i(1, sql.c_str(), db);
		
		auto x = otl_cursor::direct_exec(db, sql.c_str(), otl_exception::enabled);
		db.commit();
		//db.rlogon("sde/gmi12345@192.168.1.187:1521/arcgis"); // connect to Oracle
		//std::string sql = "select brand,model,u_number,IP from its_device where device_code='SP15-06'";
		//otl_stream i(1,sql.c_str(),db);
		//std::string brand = "";
		//std::string device_id = "";
		//std::string	device_ip = "";
		//std::string	device_model ="";
		//while (!i.eof())
		//{
		//	i >> brand >> device_model >> device_id >> device_ip;
		//}
		////auto s = UtfToGbk(device_model.c_str());
		//auto s = brand.c_str();
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