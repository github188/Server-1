// testJsoncpp.cpp : 定义控制台应用程序的入口点。
//
#include <iostream>
#include "../../thirdParty/jsoncpp/json/json.h"
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>


int main()
{
	
	std::unordered_multimap <std::string, int> type_and_requestID; //device type classification ---requestID
	std::unordered_map<int,std::unordered_map<std::string, std::string>> requestID_and_attribute; // device attribute

	 
	std::ifstream ifs;
	ifs.open("type_oid.json");
	if (!ifs.is_open())
		throw std::logic_error("no json file find");

	Json::Reader reader;
	Json::Value root;
	if (!reader.parse(ifs, root, false))
		throw std::runtime_error("parse json failed");


	int index = 0;
	int requestID = 1;
	auto type_name = root.getMemberNames();
	for (auto type : root)
	{
		auto type_name_temp = type_name[index];
		for (auto device : type)
		{
			std::unordered_map<std::string, std::string> attribute;
			type_and_requestID.emplace(type_name_temp, requestID);

			auto key_name = device.getMemberNames();
			auto iter = std::find(key_name.begin(), key_name.end(), "typeID");
			if (iter == key_name.end())
			{
				std::ostringstream message;
				message << "in " << type_name_temp << " , a object find not typeID";
				//throw std::logic_error(message.str());
			}

			for (auto key_name_temp : key_name)
			{
				auto key_value = device[key_name_temp].asString();
				//std::cout << key_name_temp << ":" << key_value << std::endl;
				attribute.emplace(key_name_temp, key_value);
			}
			requestID_and_attribute.emplace(requestID, attribute);
			++requestID;
		}
		++index;
	}
	
	//for (auto type_and_requestID_temp : type_and_requestID)
	{
		//if (type_and_requestID_temp.first == "server")
			for (auto requestID_and_attribute_temp : requestID_and_attribute)
				//if (requestID_and_attribute_temp.first == type_and_requestID_temp.second)
				{
					for (auto attribute_temp : requestID_and_attribute_temp.second)
						std::cout << attribute_temp.first << ":" << attribute_temp.second << std::endl;
					std::cout << std::endl;
				}			
	}


	/*auto type_count = root.size();
	auto type_name = root.getMemberNames();
	Json::Value server_value = root["server"];
	auto size1=server_value.size();
	auto keyname = server_value[0].getMemberNames();

	for (auto i : server_value)
	{
		auto keyname = i.getMemberNames();
		auto type = i["type"].asString();
	}
	std::cout << root;*/
	return 0;
}

