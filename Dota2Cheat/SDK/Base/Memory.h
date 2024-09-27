#pragma once
#include <cstdint>
#include <Windows.h>
#include <string>
#include <array>
#include "Address.h"
#include "Function.h"

// Converts IDA-style signatures to codepoints in a string
static std::string IDAtoBinary(std::string_view combo)
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
			pattern += (char)strtol(&combo[i], 0, 16);
			i += 2;
		}
	}
	return pattern;
}

// Utility class for working with memory
class Memory {
	// Boyer-Moore-Horspool with wildcards implementation
	static std::array<size_t, 256> FillShiftTable(std::string_view pattern, const uint8_t wildcard) {
		std::array<size_t, 256> bad_char_skip = {};
		size_t idx = 0;
		const size_t last = pattern.size() - 1;

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

	struct InterfaceInfo {
		void* (*Create)();
		const char* m_szName;
		InterfaceInfo* m_pNext;
	};

	using PatchSequence = std::vector<BYTE>;
	static inline std::map<uintptr_t, PatchSequence> patches;
public:

	static void RevertPatches() {
		static auto NtProtectVirtualMemory = Memory::GetExport("ntdll.dll", "NtProtectVirtualMemory");
		for (auto& [pAddr, sequence] : patches) {
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

	static Address ScanBinary(const std::string& pattern, const std::string& module)
	{
		using namespace std::string_view_literals;

		const auto begin = (uintptr_t)GetModuleHandleA(module.data());
		const auto pNTHeaders = PIMAGE_NT_HEADERS((uint8_t*)(begin + PIMAGE_DOS_HEADER(begin)->e_lfanew));

		// Caching .text sections for maximum performance
		// Why scan everything when you can scan code alone?
		static std::unordered_map<std::string, PIMAGE_SECTION_HEADER> textSections;

		auto textSection = textSections[module];
		if (!textSection) {
			PIMAGE_SECTION_HEADER pSectionHeader = IMAGE_FIRST_SECTION(pNTHeaders);
			for (int i = 0; i < pNTHeaders->FileHeader.NumberOfSections; ++i, ++pSectionHeader) {
				if ((char*)pSectionHeader->Name == ".text"sv) {
					textSection = textSections[module] = pSectionHeader;
					break;
				}
			}
		}

		uint8_t* scanPos = (uint8_t*)begin + textSection->VirtualAddress;
		const uint8_t* scanEnd = (uint8_t*)((uint8_t*)begin + textSection->VirtualAddress + textSection->Misc.VirtualSize - pattern.size());

		const size_t last = pattern.size() - 1;
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

	// Scans using IDA-style signature
	static Address Scan(std::string_view signature, const std::string& moduleName) {
		return ScanBinary(IDAtoBinary(signature), moduleName);
	}

	template<typename T>
	static void Copy(T* dst, T* src) {
		memcpy((void*)dst, (const void*)src, sizeof(T));
	}
	static void Copy(auto* dst, const auto* src, size_t size) {
		memcpy((void*)dst, (const void*)src, size);
	}

	// In these methods we traverse the interface list ourselves

	static Address GetInterface(const char* dllName, const char* interfaceName) {
		auto CreateInterface = GetExport(dllName, "CreateInterface");
		return CreateInterface(interfaceName, nullptr);
	}

	static Address GetInterfaceBySubstr(const char* dllName, const char* substr) {
		auto pInterface = (const InterfaceInfo*)GetExport<Address>(dllName, "CreateInterface").GetAbsoluteAddress(3).Dereference();

		while (pInterface) {
			if (std::string_view(pInterface->m_szName).find(substr) != std::string_view::npos)
				return pInterface->Create();

			pInterface = pInterface->m_pNext;
		}

		return nullptr;
	}

	template<typename T = Function>
	// Returns an exported function, if it's available
	static T GetExport(std::string_view dllName, std::string_view exportName) {
		return T(GetProcAddress(GetModuleHandleA(dllName.data()), exportName.data()));
	}

	// Returns a module's base address, for use with RVA
	static Address GetModule(std::string_view dllName) {
		return (void*)GetModuleHandleA(dllName.data());
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

Address FindStaticGameSystem(std::string_view name);
Address FindReallocatingGameSystem(std::string_view name);