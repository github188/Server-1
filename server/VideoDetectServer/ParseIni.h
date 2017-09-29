#pragma once
#include <memory>
#include <string>
#include <unordered_map>
namespace WwFoundation
{
	class ParseIni
	{
	public:
		ParseIni(const std::string& StrPath) :strPath_(StrPath) {}
		void parse();
		std::string& getValue(const std::string& section, const std::string& key);
	private:
		void trim(std::string& s);
		std::string& getSection(std::string& strText);
		void getPair(const std::string& strText, std::string &key, std::string &value);
		void addKeyValue(const std::string& section, const std::string& key, const std::string& value);
	private:
		std::string strPath_;
		std::unordered_map<std::string, std::unordered_map<std::string, std::string>> section_keyValue_;
	};
}
