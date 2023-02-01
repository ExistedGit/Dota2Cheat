#pragma once
#include "patternscan.h"
#include "Enums.h"
#include "sdk.h"
#include "Wrappers.h"
#include <map>
#include "SDK/color.h"
#include "Interfaces.h"
#include "MinHook.h"

namespace Signatures {
	typedef void(__fastcall* CMsgFn)(const char* format, ...);
	typedef void(__fastcall* ColorMsgFn)(Color* color, const char* format, ...);
	typedef void(__fastcall* prepareUnitOrdersFn)(DotaPlayer* player, DotaUnitOrder_t orderType, UINT32 targetIndex, Vector3* position, UINT32 abilityIndex, PlayerOrderIssuer_t orderIssuer, BaseEntity* issuer, bool queue, bool showEffects);
	typedef void(__fastcall* EntityCallback)(void* thisptr);
	typedef void(__fastcall* WorldToScreenFn)(Vector3 coord, int* outX, int* outY, void* offset);
	typedef unsigned char(__fastcall* EntCheckFn)(void* thisptr, uint32_t entIdx);
	typedef void(__fastcall* DestroyParticleFn)(void* thisptr, ENT_HANDLE handle, bool unk);
	typedef uintptr_t(__fastcall* EntGetter)(void* thisptr, uint32_t entIdx);
	typedef double(__fastcall* GetLevelSpecialValueForFn)(void* thisptr, int abilityIndex, const char* value, int level);
	
	inline CMsgFn CMsg = nullptr;
	inline ColorMsgFn CMsgColor = nullptr;
	// for MinHook
	inline prepareUnitOrdersFn PrepareUnitOrders = nullptr;

	inline EntityCallback OnColorChanged = nullptr;
	inline DestroyParticleFn DestroyParticle = nullptr;

	namespace Scripts {
		inline WorldToScreenFn WorldToScreen = nullptr;
		inline EntCheckFn IsRoshan = nullptr;
		inline GetLevelSpecialValueForFn GetLevelSpecialValueFor = nullptr;
//		inline EntGetter GetCastRange = nullptr;
	}

	inline void InitSignatures() {
		CMsg = reinterpret_cast<CMsgFn>(GetProcAddress(GetModuleHandleA("tier0.dll"), "Msg"));
		ColorMsgFn CMsgColor = reinterpret_cast<ColorMsgFn>(GetProcAddress(GetModuleHandleA("tier0.dll"), "?ConColorMsg@@YAXAEBVColor@@PEBDZZ"));
		
		char funcAddr[256];
		char funcAddrMask[256];
		
		//prepareUnitOrders
		ParseCombo("4C 89 4C 24 20 44 89 44 24 18 89 54 24 10 55 53 57 41 55 41 57 48 8D 6C 24 C0", funcAddr, funcAddrMask);
		PrepareUnitOrders = (prepareUnitOrdersFn)PatternScanExModule(CurProcHandle, CurProcId, L"client.dll", funcAddr, funcAddrMask);


		// I think there's a netvar for that
		//ParseCombo("48 83 EC 28 48 8B ? ? ? ? ? 48 85 C0 74 34 48 63 48 68 44 8B 80 CC 00 00 00 83 F9 0B 77 0F 48 8B C1 48 8D ? ? ? ? ? 8B 0C 81 EB 02 33 C9 3B ? ? ? ? ? 7E 0B 41 8D 40 F1 A9 FB FF FF FF 75 2A 8B CA E8 ? ? ? ? 48 85 C0 74 1E 80 B8 50 05 00 00 03 75 15 48 8D 88 E8 0B 00 00 48 8B 01 48 83 C4 28 48 FF A0 C8 01 00 00 32 C0 48 83 C4 28 C3", funcAddr, funcAddrMask);
		//Scripts::IsRoshan = (EntCheckFn)PatternScanExModule(CurProcHandle, CurProcId, L"client.dll", funcAddr, funcAddrMask);
		
		// Xref DestroyParticleEffect to a lea rcx just behind the string
		// It's offset by 9 bytes because it checks for an invalid handle before doing the initial mov
		ParseCombo("48 89 6C 24 18 56 48 83 EC 30 48 63 81 80 00 00 00 41 0F B6 E8 48 89 5C 24 40 48 8B F1", funcAddr, funcAddrMask);
		DestroyParticle = (DestroyParticleFn)
			((uintptr_t)PatternScanExModule(CurProcHandle, CurProcId, L"client.dll", funcAddr, funcAddrMask)
				- 9);

		// UnknownCheats wiki -> Dota 2 -> link to Using engine functions
		ParseCombo("56 57 41 56 48 83 EC 60 49 8B F0 4C 8B F2 48 8B F9 4D 85 C9", funcAddr, funcAddrMask);
		Scripts::WorldToScreen = (WorldToScreenFn)(
			(uintptr_t)PatternScanExModule(CurProcHandle, CurProcId, L"client.dll", funcAddr, funcAddrMask)
			- 5 // Skipping function prologue for shortening the sig
		);
		
		ParseCombo("57 48 83 EC 30 48 8B ? ? ? ? ? 41 8B D9 49 8B F8 48 85 C0 74 34", funcAddr, funcAddrMask);
		Scripts::GetLevelSpecialValueFor = (GetLevelSpecialValueForFn)(
			(uintptr_t)PatternScanExModule(CurProcHandle, CurProcId, L"client.dll", funcAddr, funcAddrMask)
				- 5 // Skipping function for shortening the sig
			);


		//ParseCombo("4C 8B 08 48 8D ? ? ? ? ? 45 33 C0 48 8B C8 48 83 C4 38 49 FF A1 98 07 00 00", funcAddr, funcAddrMask);
		//Scripts::GetCastRange = (EntGetter)
		//	((uintptr_t)PatternScanExModule(CurProcHandle, CurProcId, L"client.dll", funcAddr, funcAddrMask)
		//		- 0x75);


		const char* _funcAddr = "\x40\x53\x48\x83\xEC\x20\x48\x8B\xD9\x48\x8B\x89\x00\x00\x00\x00\x48\x8B\x01\x0F\xB6\x93";
		const char* _funcAddrMask = "xxxxxxxxxxxx????xxxxxx";
		OnColorChanged = (EntityCallback)PatternScanExModule(CurProcHandle, CurProcId, L"client.dll", _funcAddr, _funcAddrMask);

	}

	inline  void LogSignatures() {
		std::cout << "[SIGNATURES]\n";
		std::cout << "PrepareUnitOrders: " << PrepareUnitOrders << std::endl;
		std::cout << "OnColorChanged: " << OnColorChanged << std::endl;
		std::cout << "WorldToScreen: " << Scripts::WorldToScreen << std::endl;
		//std::cout << "IsRoshan: " << Scripts::IsRoshan << std::endl;
		std::cout << "DestroyParticle: " << DestroyParticle << std::endl;
	}
}
