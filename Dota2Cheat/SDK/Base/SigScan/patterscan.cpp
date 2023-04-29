#include "patternscan.h"

// Boyer-Moore-Horspool with wildcards implementation
FORCEINLINE std::array<size_t, 256> FillShiftTable(const char* pattern, const uint8_t wildcard) {
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

void* PatternScanEx(uintptr_t begin, uintptr_t end, const char* pattern)
{
	auto scanPos = (uint8_t*)begin;
	auto scanEnd = (uint8_t*)end;
	const size_t last = strlen(pattern) - 1;
	auto bad_char_skip = FillShiftTable(pattern, 0xCC);

	// Search
	for (; scanPos <= scanEnd; scanPos += bad_char_skip[scanPos[last]])
		for (size_t idx = last; idx >= 0; --idx) {
			uint8_t elem = pattern[idx];
			if (elem != 0xCC && elem != scanPos[idx])
				break;
			if (idx == 0)
				return scanPos;
		}

	return nullptr;
}

void* PatternScanInModule(const char* module, const char* pattern)
{
	MODULEENTRY32 modEntry = GetModule(GetCurrentProcessId(), module);

	if (!modEntry.th32ModuleID)
		return nullptr;

	uintptr_t begin = (uintptr_t)modEntry.modBaseAddr;
	uintptr_t end = begin + modEntry.modBaseSize - strlen(pattern);
	return PatternScanEx(begin, end, pattern);
}