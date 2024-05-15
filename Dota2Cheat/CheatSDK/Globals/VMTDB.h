#pragma once
#include <nlohmann/json.hpp>
#include <map>
#include <string>

class VMDB {
	inline static nlohmann::json Data;
public:
	static nlohmann::json* GetNestedJsonEntry(const std::string& name);

	// Use a map of VM names to index pointers to fill in their indexes
	static void ParseMap(const std::map <std::string, uint32_t*>& m);

	static void LoadFromFile(const std::string& url);;
};