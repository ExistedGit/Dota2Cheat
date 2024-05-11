#pragma once
#include <json.hpp>
#include <map>
#include <string>

class VMDB {
	inline static nlohmann::json Data;
public:
	// Use a map of VM names to index pointers to fill in their indexes
	static void ParseMap(const std::map <std::string, int*>& m);

	static void LoadFromFile(const std::string& url);;
};