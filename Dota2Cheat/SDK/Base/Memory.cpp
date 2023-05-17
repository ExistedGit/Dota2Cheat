#include "Memory.h"

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

void* Memory::PatternScanInModule(const char* module, const char* pattern)
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

std::string Memory::ParseCombo(const std::string& combo)
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

