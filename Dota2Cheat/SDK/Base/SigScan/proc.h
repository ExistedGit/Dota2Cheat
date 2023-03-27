#pragma once
#include "../../../include.h"

DWORD GetProcId(const wchar_t* procname);

uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modname);
MODULEENTRY32 GetModule(DWORD dwProcID, const wchar_t* moduleName);