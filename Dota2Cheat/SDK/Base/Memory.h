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


	template<typename T = Address>
	T GetAbsoluteAddress(int addrOffset, int opcodeSize = -1) const {
		if (opcodeSize == -1)
			opcodeSize = addrOffset + sizeof(uint32_t);
		return T(ptr + *(int*)(ptr + addrOffset) + opcodeSize);
	}

	template<typename T>
	T* As() const {
		return (T*)ptr;
	}

};

class SigScan {
	//Splits IDA pattern into code mask/pattern
	static void ParseCombo(const char* combo, std::string& pattern, std::string& mask)
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
				mask += '?';
				pattern += '\x00';
				i += 1;
			}
			else
			{
				char byte = (char)strtol(&combo[i], 0, 16);
				pattern += byte;
				mask += 'x';
				i += 2;
			}
		}
	}
public:
	static Address Find(const std::string& signature, const wchar_t* moduleName) {
		int maskLength = (signature.length() + 1) / 3 + 1;
		std::string pattern, mask;
		pattern.reserve(maskLength);
		mask.reserve(maskLength);

		ParseCombo(signature.c_str(), pattern, mask);

		Address result(PatternScanInModule(moduleName, pattern.c_str(), mask.c_str()));
		return result;
	}
};

inline void MemCopy(auto dst, auto src, size_t size) {
	memcpy((void*)dst, (const void*)src, size);
}

// Returns an exported function, if it's available
inline Function GetExport(const char* dllName, const char* exportName) {
	return Function((void*)GetProcAddress(GetModuleHandleA(dllName), exportName));
}

template<typename T = uintptr_t>
inline bool IsValidReadPtr(T p) {
	if (!p)
		return false;
	MEMORY_BASIC_INFORMATION mbi;
	memset(&mbi, 0, sizeof(mbi));
	if (!VirtualQuery((void*)p, &mbi, sizeof(mbi)))
		return false;
	if (!(mbi.State & MEM_COMMIT))
		return false;
	if (!(mbi.Protect & (PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY |
		PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY)))
		return false;
	if (mbi.Protect & (PAGE_GUARD | PAGE_NOACCESS))
		return false;
	return true;
}

template<typename T = uintptr_t>
inline bool IsValidWritePtr(T p)
{
	MEMORY_BASIC_INFORMATION mbi;
	memset(&mbi, 0, sizeof(mbi));
	if (!VirtualQuery((void*)p, &mbi, sizeof(mbi)))
		return false;
	if (!(mbi.State & MEM_COMMIT))
		return false;
	if (!(mbi.Protect & (PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY)))
		return false;
	if (mbi.Protect & (PAGE_GUARD | PAGE_NOACCESS))
		return false;
	return true;
}

template<typename T = uintptr_t>
inline bool IsValidCodePtr(T p)
{
	MEMORY_BASIC_INFORMATION mbi;
	memset(&mbi, 0, sizeof(mbi));
	if (!VirtualQuery((void*)p, &mbi, sizeof(mbi)))
		return false;
	if (!(mbi.State & MEM_COMMIT))
		return false;
	if (!(mbi.Protect & (PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY)))
		return false;
	if (mbi.Protect & (PAGE_GUARD | PAGE_NOACCESS))
		return false;
	return true;
}