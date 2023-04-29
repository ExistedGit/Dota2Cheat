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
	//Splits IDA pattern into a pattern for the BMH algorithm with 0xCC as the wildcard
	static std::string ParseCombo(const std::string& combo)
	{
		unsigned int patternLen = (combo.size() + 1) / 3;
		std::string pattern;
		pattern.reserve(patternLen);

		int index = 0;
		for (unsigned int i = 0; i < combo.size(); i++)
		{
			if (combo[i] == ' ')
			{
				continue;
			}

			else if (combo[i] == '?')
			{
				pattern += '\xCC';
				i += 1;
			}
			else
			{
				char byte = (char)strtol(&combo[i], 0, 16);
				pattern += byte;
				i += 2;
			}
		}
		return pattern;
	}
public:
	static Address Find(const std::string& signature, const std::string& moduleName) {
		return PatternScanInModule(moduleName.c_str(), ParseCombo(signature).c_str());
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