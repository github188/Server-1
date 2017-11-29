#include <codecvt>
#include <locale>
#include <vector>
#include <algorithm>
#include <memory>
#include <set>
#include <map>
#include <functional>
#include <thread>
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

struct MyStruct
{
	int a;
	int b;
	~MyStruct()
	{
		int x;
	}
};

struct MyStruct1
{
	struct MyStruct x1;
	std::string y;
	std::string z;
	bool operator ==(const struct MyStruct1& ms) const
	{
		return (ms.y == y && ms.z == z);
	}
};

class Point
{
private:
	int x;
public:
	Point(){}
	Point(int x1)
	{
		x = x1;
	}
	const int operator()(int x);
	void a()
	{
		Point(3);
	}
};

const int Point::operator()(int x)
{
	printf("%d\n", x);
	return 0;
}

int main()
{
	std::vector<std::string> m_str[2];
	m_str[0].push_back("1");
	m_str[1].push_back("2");

	auto x = m_str[0][0];

	
	


	std::map<int, MyStruct> map77;
	
	MyStruct MyStruct78;
	MyStruct78.a = 1;
	MyStruct78.b = 2;
	map77.emplace(1, MyStruct78);
	auto first = map77.begin();
	auto first_value = first->second;
	map77.clear();


	std::vector<int> vec73;
	vec73.emplace_back(1);

	wchar_t * w_p =  L"12";
	std::map<int, bool> s;
	s[1];
	auto iter = s.begin();
	


	{
		std::shared_ptr<int> p(new int(5));
		auto p1 = std::move(p);
	}
	std::vector<int> vec1{ 1,2,4,3 };
	std::vector<int> vec2{ 1,2,3,4 };
	
	if (vec1 == vec2)
		int a = 3;


	std::vector<int> dd{ 1,2,3,4,5 };
	std::for_each(dd.begin(), dd.end(), Point());

	Point()(4);
	struct MyStruct temp1 {1,1};
	struct MyStruct temp2 { 2, 2 };

	struct MyStruct1 zzz1 { temp1, "1", "2" };
	struct MyStruct1 zzz2 { temp2, "1", "2" };

	if (zzz1 == zzz2)
	{
		int a = 2;
	}


	std::string gbk = "½ÜÂõ";
	auto gbk_str = gbk.c_str();
	printf("%s", gbk_str);
	auto gbk_str1 = gbk.data();
	printf("%s", gbk_str1);
	auto utf8 = gb2312_to_utf8(gbk);
	auto utf8_str = utf8.c_str();

	std::set<int> test_set{ 1 };
	auto iter = test_set.find(1);
	auto p = &(*iter);
}
