#pragma once
#include "sdk.h"
#include "patternscan.h"



namespace Globals {
	class CDOTAGameRules {
	public:

	};

	CDOTAGameRules* GameRules = nullptr;

	void InitGlobals() {
		char funcAddr[60];
		char funcAddrMask[60];
		ParseCombo("48 8B ? ? ? ? ? 48 85 C0 74 34 48 63 48 68 44 8B 90 CC 00 00 00 83 F9 0B", funcAddr, funcAddrMask);
		uintptr_t addr = (uintptr_t)PatternScanExModule(CurProcHandle, CurProcId, L"client.dll", funcAddr, funcAddrMask);
		std::cout << "[TEST] addr " << addr << '\n';
		if (addr != 0);
			GameRules = *(CDOTAGameRules**)GetAbsoluteAddress(addr, 3, 7);
	}
	void LogGlobals() {
		std::cout << "[GLOBALS]\n";
		std::cout << std::hex << "GameRules: " << GameRules << '\n';
	}
}