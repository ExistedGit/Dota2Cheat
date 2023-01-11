#pragma once
#include "sdk.h"
#include "patternscan.h"
namespace Globals {
	class CDOTAGameRules {
	public:
	};

	CDOTAGameRules* GameRules;

	void InitGlobals() {
		char funcAddr[60];
		char funcAddrMask[60];
		ParseCombo("48 83 EC 28 8B 02 83 F8 02 7D 12 48 8D 0D ? ? ? ? 48 83 C4 28 48 FF 25 ? ? ? ? 48 89 6C", funcAddr, funcAddrMask);
		DWORD id = GetCurrentProcessId();
		HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, TRUE, id);
		uintptr_t addr = (uintptr_t)PatternScanExModule(handle, id, L"client.dll", funcAddr, funcAddrMask);
		GameRules = GetAbsoluteAddress(addr, 3, 7);
	}
}