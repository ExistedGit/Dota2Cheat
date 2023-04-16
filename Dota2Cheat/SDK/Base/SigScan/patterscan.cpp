#include "patternscan.h"

//Internal Pattern Scan
void* PatternScan(char* base, size_t size, const char* pattern, const char* mask)
{
	size_t patternLength = strlen(mask);

	for (unsigned int i = 0; i < size - patternLength; ++i)
	{
		bool found = true;
		for (unsigned int j = 0; j < patternLength; ++j)
		{
			if (mask[j] != '?' && pattern[j] != *(base + i + j))
			{
				found = false;
				break;
			}
		}

		if (found)
			return (void*)(base + i);
	}
	return nullptr;
}

//External Wrapper
void* PatternScanEx(uintptr_t begin, uintptr_t end, const char* pattern, const char* mask)
{
	uintptr_t currentChunk = begin;
	size_t patternSize = strlen(pattern);
	while (currentChunk < end)
	{
		void* internalAddress = PatternScan((char*)currentChunk, patternSize + 1, pattern, mask);

		if (internalAddress)
		{
			//calculate from internal to external
			uintptr_t offsetFromBuffer = (uintptr_t)internalAddress - currentChunk;
			return (void*)(currentChunk + offsetFromBuffer);
		}
		else
			//advance to next chunk
			currentChunk++;
	}
	return nullptr;
}

//Module wrapper for external pattern scan
void* PatternScanInModule(const wchar_t* module, const char* pattern, const char* mask)
{
	MODULEENTRY32 modEntry = GetModule(GetCurrentProcessId(), module);

	if (!modEntry.th32ModuleID)
		return nullptr;

	uintptr_t begin = (uintptr_t)modEntry.modBaseAddr;
	uintptr_t end = begin + modEntry.modBaseSize;
	return PatternScanEx(begin, end, pattern, mask);
}