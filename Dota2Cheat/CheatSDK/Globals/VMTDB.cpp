#include <fstream>
#include <Base/Logging.h>
#include "VMTDB.h"

void VMDB::ParseMap(const std::map<std::string, int*>& m) {
	for (auto& [name, val] : m) {
		if (!Data.contains(name)) {
			LogF(LP_ERROR, "VM index '{}' not found in JSON data!", name);
			continue;
		}

		*val = Data[name];
	}
}

void VMDB::LoadFromFile(const std::string& url) {
	if (std::ifstream fin(url); fin.is_open()) {
		LogF(LP_INFO, "Loading VM indices from {}\n", url);
		Data = nlohmann::json::parse(fin);
		fin.close();
	}
}
