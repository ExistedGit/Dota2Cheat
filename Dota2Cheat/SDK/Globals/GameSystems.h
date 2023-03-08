#pragma once
#include "../../include.h"
#include "Interfaces.h"
#include "Context.h"

#include "../SigScan/patternscan.h"
#include "../GameSystems/C_DOTAGameRules.h"
#include "../GameSystems/C_DOTA_PlayerResource.h"
#include "../GameSystems/CDOTAParticleManager.h"
#include "../GameSystems/C_DOTA_ProjectileManager.h"
#include "../GameSystems/CGameEventManager.h"
#include "../Base/Memory.h"


namespace GameSystems {

	//GameRules is nullptr while not in a game
	//So we need to dereference it when we enter a match and reset to nullptr on leave
	inline CDOTAGameRules** GameRulesPtr{};
	inline CDOTAGameRules* GameRules{};

	inline C_DOTA_PlayerResource** PlayerResourcePtr{};
	inline C_DOTA_PlayerResource* PlayerResource{};

	inline CDOTAParticleManager** ParticleManagerPtr{};
	inline CDOTAParticleManager* ParticleManager{};

	inline C_DOTA_ProjectileManager** ProjectileManagerPtr{};
	inline C_DOTA_ProjectileManager* ProjectileManager{};

	inline CGameEventManager** GameEventManagerPtr{};
	inline CGameEventManager* GameEventManager{};

	inline void FindGameSystems() {
		std::cout << "[GLOBAL POINTERS]\n";

		// In Source2Client::Init(), right after "g_GameEventManager.Init()":
		// mov rcx, [XXXXXXXXX]
		GameEventManagerPtr = (CGameEventManager**)GetAbsoluteAddress(
			(uintptr_t)Interfaces::Client->GetVFunc(3).ptr + 0x106,
			3,
			7);

		std::cout << "GameEventManagerPtr: " << GameEventManagerPtr  << '\n';
		// GetProjectileManager, it's too short and its caller is too generic to be sigged
		// xref "Spews a list of all client-side projectiles", above it is lea rax, [XXXXXXXX]
		ProjectileManagerPtr =
			(decltype(ProjectileManagerPtr))
			GetAbsoluteAddress(
				(uintptr_t)GetModuleHandleA("client.dll") + 0x1330380,
				3,
				7
			);
		std::cout << "ProjectileManagerPtr: " << ProjectileManagerPtr << '\n';

		char funcAddr[60];
		char funcAddrMask[60];

		ParseCombo("48 89 5C 24 18 48 89 6C 24 20 57 48 83 EC 20 48 8B ? ? ? ? ? 8B DA 48 8B E9 48 85 FF", funcAddr, funcAddrMask);
		uintptr_t addr = (uintptr_t)PatternScanExModule(ctx.CurProcHandle, ctx.CurProcId, L"client.dll", funcAddr, funcAddrMask) + 0xF;
		GameRulesPtr = (CDOTAGameRules**)GetAbsoluteAddress(addr, 3, 7);
		std::cout << "GameRulesPtr: " << GameRulesPtr << '\n';

		ParseCombo("48 8B ? ? ? ? ? 48 85 C9 0F 85 ? ? ? ? B8 FF FF FF FF C3", funcAddr, funcAddrMask);
		addr = (uintptr_t)PatternScanExModule(ctx.CurProcHandle, ctx.CurProcId, L"client.dll", funcAddr, funcAddrMask);
		PlayerResourcePtr = (C_DOTA_PlayerResource**)GetAbsoluteAddress(addr, 3, 7);
		std::cout << "PlayerResourcePtr: " << PlayerResourcePtr << '\n';

		//ParseCombo("48 8B ? ? ? ? ? 33 C9 48 89 5C 24 30 48 8B 18 E8 ? ? ? ? 48 8B ? ? ? ? ? 4C 8B C0 48 8B D7", funcAddr, funcAddrMask);
		//ScriptVMPtr = (IScriptVM**)GetAbsoluteAddress((uintptr_t)PatternScanExModule(ctx.CurProcHandle, ctx.CurProcId, L"client.dll", funcAddr, funcAddrMask), 3, 7);
		//std::cout << "SVMPtr: " << ScriptVMPtr << '\n';

		ParseCombo("7E 0B 41 8D 42 F1 A9 FB FF FF FF 75 0B 41 8B C9 E8 ? ? ? ? 48 8B D8 E8", funcAddr, funcAddrMask);
		addr = GetAbsoluteAddress(
			(uintptr_t)PatternScanExModule(ctx.CurProcHandle,
				ctx.CurProcId,
				L"client.dll",
				funcAddr,
				funcAddrMask) + 0x18,
			1,
			5
		);
		ParticleManagerPtr = (CDOTAParticleManager**)GetAbsoluteAddress(addr, 3, 7);
		std::cout << "DPMP: " << ParticleManagerPtr << '\n';
	}
	inline void LogGameSystems() {
		std::cout << "[GLOBALS]\n";
		std::cout << "GameRules: " << GameRules << '\n';
		std::cout << "Projectile Manager:" << ProjectileManager << '\n';
		std::cout << "Particle Manager: " << ParticleManager << ' ' << ParticleManager->GetVFunc(7).ptr << "\n";
	}
}