#pragma once
#include <cstdint>
#include "SigScan/patternscan.h"


class Address {
public:
	uintptr_t ptr;

	Address(uintptr_t ptr) : ptr(ptr) {}
	Address(void* ptr) : ptr((uintptr_t)ptr) {}

	template<typename T>
	operator T() const
	{
		return (T)ptr;
	}
	
	Address Offset(int offset) {
		return Address(ptr + offset);
	}

	template<typename T>
	T GetAbsoluteAddress(int opcodeOffset, int addrOffset, int opcodeSize) const {
		uintptr_t opcodeAddr = ptr + opcodeOffset;
		return T(opcodeAddr + *(int*)(opcodeAddr + addrOffset) + opcodeSize);
	}

	Address GetAbsoluteAddress(int opcodeOffset, int addrOffset, int opcodeSize) const {
		uintptr_t opcodeAddr = ptr + opcodeOffset;
		return Address(opcodeAddr + *(int*)(opcodeAddr + addrOffset) + opcodeSize);
	}

	template<typename T>
	T* As() const {
		return (T*)ptr;
	}

};

class SigScanContext {
	//Split combo pattern into mask/pattern
	void ParseCombo(const char* combo, char* pattern, char* mask)
	{
		unsigned int patternLen = (strlen(combo) + 1) / 3;
		int index = 0;
		for (unsigned int i = 0; i < strlen(combo); i++)
		{
			if (combo[i] == ' ')
			{
				continue;
			}

			else if (combo[i] == '?')
			{
				mask[index] = '?';
				pattern[index++] = '\x00';
				i += 1;
			}
			else
			{
				char byte = (char)strtol(&combo[i], 0, 16);
				pattern[index] = byte;
				mask[index++] = 'x';
				i += 2;
			}
		}
		pattern[index] = '\0';
		mask[index] = '\0';
	}
public:
	HANDLE procHandle;
	int pid;

	SigScanContext(HANDLE procHandle, int pid) : procHandle(procHandle), pid(pid) {

	}

	Address Scan(const std::string& signature, const wchar_t* moduleName) {
		int maskLength = (signature.length() + 1) / 3;
		char* pattern = new char[maskLength],
			* mask = new char[maskLength];

		ParseCombo(signature.c_str(), pattern, mask);

		return Address(PatternScanExModule(procHandle, pid, moduleName, pattern, mask));
	}
};

template<typename T, typename Z>
void MemCopy(T dst, Z src, size_t size) {
	memcpy((void*)dst, (const void*)src, size);
}