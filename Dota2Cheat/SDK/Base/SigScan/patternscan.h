#pragma once
#include "proc.h"

void* PatternScanEx(uintptr_t begin, uintptr_t end, const char* pattern);

void* PatternScanInModule(const char* module, const char* pattern);