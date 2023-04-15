#include "GameSystems.h"

#define SET_VAR(var, data) var = (data); \
if(var) \
	LogF(LP_DATA, "{}: {}", #var, (void*)var); \
else \
	LogF(LP_ERROR, "{}: {}", #var, (void*)var);

void GameSystems::FindGameSystems() {
	Log(LP_INFO, "GAME SYSTEM POINTERS:");

	// In Source2Client::Init(), right after "g_GameEventManager.Init()":
	// mov rcx, [XXXXXXXXX]
	SET_VAR(GameEventManagerPtr, Address(Interfaces::Client->GetVFunc(3).ptr).Offset(0x106).GetAbsoluteAddress(3, 7));

	// CSource2Client::NotifySignon
	SET_VAR(RichPresence, Address(Interfaces::Client->GetVFunc(47).ptr)
		.Offset(0x23)
		.GetAbsoluteAddress(1)
		.GetAbsoluteAddress(3));

	//xref: "Failed to get custom game %llu timestamp/CRC!\n"
	SET_VAR(GCClientSystem,
		SigScan::Find("E8 ? ? ? ? 48 8B D7 48 8B CB E8 ? ? ? ? 48 8D 8B", L"client.dll")
		.GetAbsoluteAddress(1)
		.Offset(0x277)
		.GetAbsoluteAddress(3));

	//xref: "activategameui", first lea rax, [XXXXXXXXX]
	//console command ^
	SET_VAR(GameUI, SigScan::Find("E8 ? ? ? ? 48 8B C8 E8 ? ? ? ? 48 8B 3D", L"client.dll")
		.GetAbsoluteAddress(1)
		.GetAbsoluteAddress(3));

	// xref "Spews a list of all client-side projectiles", above it is lea rax, [XXXXXXXX]
	// right click -> Find references to -> Address: XXXXXXXX
	SET_VAR(ProjectileManagerPtr,
		SigScan::Find("40 55 53 41 57 48 8D 6C 24 D0 48 81 EC 30 01 00 00 48 89 BC 24 58 01 00 00", L"client.dll")
		.Offset(0x95e)
		.GetAbsoluteAddress(3, 7));

	SET_VAR(GameRulesPtr, Address(CDOTAGameRules::GetGameTimeFunc).Offset(0xF).GetAbsoluteAddress(3, 7));
	
	SET_VAR(PlayerResourcePtr, SigScan::Find("48 8B ? ? ? ? ? 48 85 C9 0F 85 ? ? ? ? B8 FF FF FF FF C3", L"client.dll")
		.GetAbsoluteAddress(3, 7));

	// xref: "flOffset"
	// lea rcx, [XXXXXXXXX] above it is GetParticleManager()
	SET_VAR(ParticleManagerPtr, SigScan::Find("E8 ? ? ? ? 8B 14 9F", L"client.dll")
		.GetAbsoluteAddress(1, 5)
		.GetAbsoluteAddress(3, 7));
}

void GameSystems::LogGameSystems() {
	Log(LP_INFO, "GAME SYSTEMS:");
	Log(LP_DATA, "GameRules: ", GameRules);
	Log(LP_DATA, "Projectile Manager: ", ProjectileManager);
	Log(LP_DATA, "CDOTARichPresence: ", RichPresence);
	LogF(LP_DATA, "Particle Manager: {} {}", (void*)ParticleManager, ParticleManager->GetVFunc(VTableIndexes::CDOTAParticleManager::CreateParticle).ptr);
}
