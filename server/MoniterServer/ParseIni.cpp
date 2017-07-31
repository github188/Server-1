#include "ParseIni.h"
#include <iostream>
#include <fstream>
#include <string>

void WwFoundation::ParseIni::parse()
{
	std::string strLine = "";
	std::string strSection = "";
	std::string strKey = "";
	std::string strValue = "";

	std::ifstream in;
	in.open(strPath_);
	if (!in.is_open())
		throw std::runtime_error("open json file failed, maybe the file is not existent");
	std::string section;
	while (!in.eof())
	{
		getline(in, strLine);
		trim(strLine);
		if (strLine == "")
			continue;
		if (strLine.at(0) == '[')
			section = getSection(strLine);
		else
		{
			getPair(strLine, strKey, strValue);
			addKeyValue(section,strKey, strValue);
		}
	}
	in.close();
}

std::string& WwFoundation::ParseIni::getValue(const std::string section, const std::string key)
{
	return section_keyValue_[section][key];
}

void WwFoundation::ParseIni::trim(std::string & s)
{
	if (s != "")
	{
		s.erase(0, s.find_first_not_of(" "));
		if (s != "")
			s.erase(s.find_last_not_of("") + 1);
	}
}

std::string & WwFoundation::ParseIni::getSection(std::string & strText)
{
	strText.erase(0, strText.find_first_not_of("["));
	strText.erase(strText.find_last_not_of("]") + 1);
	return strText;
}

void WwFoundation::ParseIni::getPair(const std::string strText, std::string & key, std::string & value)
{
	int pos = strText.find("=");
	key = strText.substr(0, pos);
	value = strText.substr(pos + 1);
	trim(key);
	trim(value);
}

void WwFoundation::ParseIni::addKeyValue(const std::string section, const std::string key, const std::string value)
{
	auto iter = section_keyValue_.find(section);
	if (iter != section_keyValue_.end())
		iter->second.emplace(std::make_pair(key, value));
	else
	{
		std::unordered_map<std::string, std::string> key_value_temp;
		key_value_temp.emplace(std::make_pair(key, value));
		section_keyValue_[section] = key_value_temp;
	}
}


