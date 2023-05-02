#pragma once
#include <Windows.h>
#include <cstdint>
#include <array>
#include <string>
#include "Address.h"

void* _PatternScan(uintptr_t begin, uintptr_t end, const char* pattern);

void* PatternScanInModule(const char* module, const char* pattern);

class SigScan {
	//Converts IDA pattern to code-style for the BMH algorithm with 0xCC as the wildcard
	static std::string ParseCombo(const std::string& combo);
public:
	FORCEINLINE static Address Find(const std::string& signature, const std::string& moduleName) {
		return PatternScanInModule(moduleName.c_str(), ParseCombo(signature).c_str());
	}
};
