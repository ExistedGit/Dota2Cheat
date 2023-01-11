#pragma once
#include "proc.h"
//Split combo pattern into mask/pattern
inline void ParseCombo(const char* combo, char* pattern, char* mask)
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
//Internal Pattern Scan
void* PatternScan(char* base, size_t size, const char* pattern, const char* mask);

//External Wrapper
void* PatternScanEx(HANDLE hPRocess, uintptr_t begin, uintptr_t end, const char* pattern, const char* mask);

//Module wrapper for external pattern scan
void* PatternScanExModule(HANDLE hProcess, DWORD processID, const wchar_t* module, const char* pattern, const char* mask);