#include <fstream>
#include <Base/Logging.h>
#include "VMTDB.h"

nlohmann::json* VMDB::GetNestedJsonEntry(const std::string& name) {
	using json = nlohmann::json;
	const static std::string delimiter = "::";
	auto nameCopy = name;
	json* entryPtr = nullptr;
	{

		size_t pos = 0;
		std::string token;
		while ((pos = nameCopy.find(delimiter)) != std::string::npos) {
			token = nameCopy.substr(0, pos);
			if (!entryPtr)
				entryPtr = &Data[token];
			else
				entryPtr = &((*entryPtr)[token]);

			nameCopy.erase(0, pos + delimiter.length());
		}
	}
	if (!entryPtr)
		entryPtr = &Data[name];
	else
		entryPtr = &((*entryPtr)[nameCopy]);
	return entryPtr;
}

void VMDB::ParseMap(const std::map<std::string, uint32_t*>& m) {
	for (auto& [name, val] : m) {
		auto& entry = *GetNestedJsonEntry(name);
		if (entry.is_null()) {
			LogF(LP_ERROR, "VM index '{}' not found in JSON data!", name);
			continue;
		}

		*val = entry;
	}
}

void VMDB::LoadFromFile(const std::string& url) {
	if (std::ifstream fin(url); fin.is_open()) {
		LogF(LP_INFO, "Loading VM indices from {}\n", url);
		Data = nlohmann::json::parse(fin);
		fin.close();
	}
}
