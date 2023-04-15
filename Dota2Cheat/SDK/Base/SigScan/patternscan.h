#pragma once
#include "proc.h"

//Internal Pattern Scan
void* PatternScan(char* base, size_t size, const char* pattern, const char* mask);

//External Wrapper
void* PatternScanEx(uintptr_t begin, uintptr_t end, const char* pattern, const char* mask);

void* PatternScanInModule(const wchar_t* module, const char* pattern, const char* mask);