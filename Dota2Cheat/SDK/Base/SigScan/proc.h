#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <string_view>

DWORD GetProcId(const char* procname);

uintptr_t GetModuleBaseAddress(DWORD procId, const char* modname);
MODULEENTRY32 GetModule(DWORD dwProcID, const char* moduleName);