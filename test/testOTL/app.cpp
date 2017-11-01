#define OTL_ORA12C_R2 // Compile OTL 4.0/OCI12
#define OTL_STL // Enable STL compatibility mode
#include "windows.h"
//#define OTL_UNICODE
#define OTL_ORA_UTF8
#include "otlv4.h" // include the OTL 4.0 header file
#include <vector>
#include <iostream>
#include <codecvt>
#include <locale>

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
	//SetEnvironmentVariable(L"NLS_LANG", L"SIMPLIFIED CHINESE_CHINA.AL32UTF8");
	SetEnvironmentVariable(L"NLS_LANG", L"SIMPLIFIED CHINESE_CHINA.ZHS16GBK");

	otl_connect db; // connect object
	otl_connect::otl_initialize(); // initialize OCI environment
	try
	{
		//db.rlogon("wangwei/111111@192.168.1.187:1521/arcgis"); // connect to Oracle

		//db.rlogon("wangwei/111111@192.168.1.192:1521/abcdef"); // connect to Oracle
		//std::string sql = "select model,u_number,IP from its_device where IP='192.168.1.20'";
		//std::string sql = "update its_device set model='°²' where IP='192.168.1.20'";

		//db.rlogon("wangwei/111111@192.168.1.192:1521/abcdef"); // connect to Oracle
		//std::string brand = "½Ü";
		//auto brand_str = brand.c_str();
		//auto utf8_brand = GBKToUTF8(brand);
		//auto utf8_brand_str = utf8_brand.c_str();
		//std::string sql = "update its_device set brand='";
		//std::string sql1 = "' where type='57'";
		//sql.append(utf8_brand).append(sql1);




		/*std::string test = "½ÜÂõ¹þ";
		auto test_str = test.c_str();
		auto utf8_test = GBKToUTF8(test);
		std::string sql2 = "insert into its_device (u_number) values('" + utf8_test + "')";
		otl_stream i(1, sql2.c_str(), db);*/
		
		//auto x = otl_cursor::direct_exec(db, sql.c_str(), otl_exception::enabled);
		//db.commit();
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

		//SetEnvironmentVariable(L"NLS_LANG", L"SIMPLIFIED CHINESE_CHINA.ZHS16GBK");
		/*std::string sql_select = "select u_number from its_device";
		otl_stream i_select(1,sql_select.c_str(),db);
		
		std::string device_id = "";
		while (!i_select.eof())
		{
			i_select >> device_id ;
		}
		auto str = device_id.c_str();*/
		db.rlogon("sde/gmi12345@192.168.1.187:1521/arcgis"); // connect to Oracle
		std::string sql = "update its_device set device_code='°²' where type='58'";
		otl_stream i_select(1, sql.c_str(), db);
		db.commit();
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