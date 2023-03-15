#pragma once
#include "proc.h"

//Internal Pattern Scan
void* PatternScan(char* base, size_t size, const char* pattern, const char* mask);

//External Wrapper
void* PatternScanEx(HANDLE hPRocess, uintptr_t begin, uintptr_t end, const char* pattern, const char* mask);

//Module wrapper for external pattern scan
void* PatternScanExModule(HANDLE hProcess, DWORD processID, const wchar_t* module, const char* pattern, const char* mask);