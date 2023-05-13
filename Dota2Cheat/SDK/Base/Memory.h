#pragma once
#include <cstdint>
#include <Windows.h>
#include "Address.h"

struct Memory {
	// Byte patching!
	template<size_t replSize>
	static void Patch(Address addr, BYTE const (&replacement)[replSize]) {
		MEMORY_BASIC_INFORMATION mbi;
		VirtualQuery(addr, &mbi, sizeof(mbi));
		VirtualProtect(mbi.BaseAddress, mbi.RegionSize, PAGE_EXECUTE_READWRITE, &mbi.Protect);

		memcpy(addr, replacement, replSize);

		VirtualProtect(mbi.BaseAddress, mbi.RegionSize, mbi.Protect, &mbi.Protect);
	}
};

// Utility functions for working with memory

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