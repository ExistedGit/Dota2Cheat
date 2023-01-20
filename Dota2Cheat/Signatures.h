#pragma once
#include "patternscan.h"
#include "Enums.h"
#include "sdk.h"

namespace Signatures {
	
	typedef void(__fastcall* prepareUnitOrdersFn)(DotaPlayer* player, DotaUnitOrder_t orderType, UINT32 targetIndex, Fvector* position, UINT32 abilityIndex, PlayerOrderIssuer_t orderIssuer, BaseEntity* issuer, bool queue, bool showEffects);
	typedef void(__fastcall* EntityCallback)(void* thisptr);
	prepareUnitOrdersFn PrepareUnitOrders = nullptr;
	EntityCallback OnColorChanged = nullptr;

	void InitSignatures() {
		//prepareUnitOrders
		{
			char funcAddr[60];
			char funcAddrMask[60];
			ParseCombo("4C 89 4C 24 20 44 89 44 24 18 89 54 24 10 55 53 57 41 55 41 57 48 8D 6C 24 C0", funcAddr, funcAddrMask);
			PrepareUnitOrders = (prepareUnitOrdersFn)PatternScanExModule(CurProcHandle,CurProcId, L"client.dll", funcAddr, funcAddrMask);
		}
		{
			const char* funcAddr = "\x40\x53\x48\x83\xEC\x20\x48\x8B\xD9\x48\x8B\x89\x00\x00\x00\x00\x48\x8B\x01\x0F\xB6\x93";
			const char* funcAddrMask = "xxxxxxxxxxxx????xxxxxx";
			OnColorChanged = (EntityCallback)PatternScanExModule(CurProcHandle, CurProcId, L"client.dll", funcAddr, funcAddrMask);
		}
	}

	void LogSignatures() {
		std::cout << "[SIGNATURES]\n";
		std::cout << "PrepareUnitOrders: " << PrepareUnitOrders << std::endl;
		std::cout << "OnColorChanged: " << OnColorChanged << std::endl;
	}
}
