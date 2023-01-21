#pragma once
#include "sdk.h"
#include "patternscan.h"
#include "Enums.h"
#include "Wrappers.h"
#include "ValveClasses.h"
namespace Globals {

	//GameRules is nullptr while not in a game
	//So we need to dereference it when we enter a match and reset to nullptr on leave
	CDOTAGameRules** GameRulesPtr = nullptr; 
	CDOTAGameRules* GameRules = nullptr;


	inline void InitGlobals() {
		
		char funcAddr[60];
		char funcAddrMask[60];
		ParseCombo("48 8B ? ? ? ? ? 48 85 C0 74 34 48 63 48 68 44 8B 90 CC 00 00 00 83 F9 0B", funcAddr, funcAddrMask);
		uintptr_t addr = (uintptr_t)PatternScanExModule(CurProcHandle, CurProcId, L"client.dll", funcAddr, funcAddrMask);
		if (addr != 0) {
			GameRulesPtr = (CDOTAGameRules**)GetAbsoluteAddress(addr, 3, 7);
			
			std::cout << "GameRulesPtr: "<<std::hex << GameRulesPtr << '\n';
			
//			if (GameRules != nullptr)
//				std::cout << "GAME STATE: " << std::dec << (int)GameRules->GetGameState() << '\n';
		}
	}
	inline void LogGlobals() {
		std::cout << "[GLOBALS]\n";
		std::cout << std::hex << "GameRules: " << GameRules << '\n';
	}
}