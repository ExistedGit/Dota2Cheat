#pragma once
#include <Windows.h>
#include <cstdint>
#include <array>
#include <string>
#include "Address.h"



class SigScan {
	//Converts IDA pattern to code-style for the BMH algorithm with 0xCC as the wildcard
	static std::string ParseCombo(const std::string& combo);
	static void* PatternScanInModule(const char* module, const char* pattern);
public:
	FORCEINLINE static Address Find(const std::string& signature, const std::string& moduleName) {
		return PatternScanInModule(moduleName.c_str(), ParseCombo(signature).c_str());
	}
};
