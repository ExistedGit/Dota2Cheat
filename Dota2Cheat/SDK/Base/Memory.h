#pragma once
#include <cstdint>
#include <Windows.h>
#include <string>
#include <array>
#include "Address.h"
#include "Function.h"

// Utility class for working with memory
class Memory {
	// Boyer-Moore-Horspool with wildcards implementation
	static std::array<size_t, 256> FillShiftTable(const char* pattern, const uint8_t wildcard) {
		std::array<size_t, 256> bad_char_skip = {};
		size_t idx = 0;
		const size_t last = strlen(pattern) - 1;

		// Get last wildcard position
		for (idx = last; idx > 0 && (uint8_t)pattern[idx] != wildcard; --idx);
		size_t diff = last - idx;
		if (diff == 0)
			diff = 1;

		// Prepare shift table
		for (idx = 0; idx < 256; ++idx)
			bad_char_skip[idx] = diff;
		for (idx = last - diff; idx < last; ++idx)
			bad_char_skip[(uint8_t)pattern[idx]] = last - idx;
		return bad_char_skip;
	}

	static std::string ParseCombo(const std::string& combo)
	{
		const size_t patternLen = (combo.size() + 1) / 3;
		std::string pattern;
		pattern.reserve(patternLen);

		int index = 0;
		for (unsigned int i = 0; i < combo.size(); i++)
		{
			if (combo[i] == ' ')
				continue;
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

	static void* PatternScanInModule(const char* module, const char* pattern)
	{
		const auto begin = (uintptr_t)GetModuleHandleA(module);

		const auto pDosHeader = PIMAGE_DOS_HEADER(begin);
		const auto pNTHeaders = PIMAGE_NT_HEADERS((uint8_t*)(begin + pDosHeader->e_lfanew));
		const auto dwSizeOfImage = pNTHeaders->OptionalHeader.SizeOfImage;

		uint8_t* scanPos = (uint8_t*)begin;
		const uint8_t* scanEnd = (uint8_t*)(begin + dwSizeOfImage - strlen(pattern));

		const size_t last = strlen(pattern) - 1;
		const auto bad_char_skip = FillShiftTable(pattern, 0xCC);

		// Search
		for (; scanPos <= scanEnd; scanPos += bad_char_skip[scanPos[last]])
			for (size_t idx = last; idx >= 0; --idx) {
				const uint8_t elem = pattern[idx];
				if (elem != 0xCC && elem != scanPos[idx])
					break;
				if (idx == 0)
					return scanPos;
			}

		return nullptr;
	}

	struct InterfaceInfo {
		void*(*Create)();
		const char* m_szName;
		InterfaceInfo* m_pNext;
	};

	using PatchSequence = std::vector<BYTE>;
	static inline std::map<uintptr_t, PatchSequence> patches;
public:

	static void RevertPatches() {
		static auto NtProtectVirtualMemory = Memory::GetExport("ntdll.dll", "NtProtectVirtualMemory");
		for (auto& [ pAddr, sequence] : patches) {
			Address addr = pAddr;
			MEMORY_BASIC_INFORMATION mbi;
			VirtualQuery(addr, &mbi, sizeof(mbi));
			
			NtProtectVirtualMemory(GetCurrentProcess(), &mbi.BaseAddress, (unsigned long*)&mbi.RegionSize, PAGE_EXECUTE_READWRITE, &mbi.Protect);
			memcpy(addr, sequence.data(), sequence.size());
			NtProtectVirtualMemory(GetCurrentProcess(), &mbi.BaseAddress, (unsigned long*)&mbi.RegionSize, mbi.Protect, &mbi.Protect);
		}
	}

	// Byte patching!
	template<size_t replSize>
	static void Patch(Address addr, BYTE const (&replacement)[replSize]) {
		MEMORY_BASIC_INFORMATION mbi;
		VirtualQuery(addr, &mbi, sizeof(mbi));

		// VirtualProtect is monitored by gameoverlayrenderer64.dll so we'll use this
		static auto NtProtectVirtualMemory = Memory::GetExport("ntdll.dll", "NtProtectVirtualMemory");
		NtProtectVirtualMemory(GetCurrentProcess(), &mbi.BaseAddress, (unsigned long*)&mbi.RegionSize, PAGE_EXECUTE_READWRITE, &mbi.Protect);
		memcpy(addr, replacement, replSize);
		NtProtectVirtualMemory(GetCurrentProcess(), &mbi.BaseAddress, (unsigned long*)&mbi.RegionSize, mbi.Protect, &mbi.Protect);

		PatchSequence data;
		data.assign(replacement, replacement + replSize);
		if (!patches.contains(addr)) // not to break the reversion for dynamic patches
			patches[addr] = data;
	}

	static Address Scan(const std::string& signature, const std::string& moduleName) {
		auto res = PatternScanInModule(moduleName.c_str(), ParseCombo(signature).c_str());

		if (!res)
			LogF(LP_ERROR, "{}: {} | BROKEN SIGNATURE", moduleName, signature);

		return res;
	}

	template<typename T>
	static void Copy(T* dst, T* src) {
		memcpy((void*)dst, (const void*)src, sizeof(T));
	}
	static void Copy(auto* dst, const auto* src, size_t size) {
		memcpy((void*)dst, (const void*)src, size);
	}

	template<typename T = void>
	static T* GetInterface(const char* dllName, const char* interfaceName) {
		auto CreateInterface = GetExport(dllName, "CreateInterface");
		return (T*)CreateInterface(interfaceName, nullptr);
	}

	template<typename T = void>
	static T* GetInterfaceBySubstr(const char* dllName, const char* substr) {
		auto pInterface = *Memory::GetExport<Address>(dllName, "CreateInterface").GetAbsoluteAddress<InterfaceInfo**>(3);

		while (pInterface) {
			if (strstr(pInterface->m_szName, substr))
				return (T*)pInterface->Create();

			pInterface = pInterface->m_pNext;
		}

		return nullptr;
	}

	template<typename T = Function>
	// Returns an exported function, if it's available
	static T GetExport(const char* dllName, const char* exportName) {
		return T(GetProcAddress(GetModuleHandleA(dllName), exportName));
	}

	// Returns a module's base address, for use with RVA
	static Address GetModule(const char* dllName) {
		return (void*)GetModuleHandleA(dllName);
	}
};



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