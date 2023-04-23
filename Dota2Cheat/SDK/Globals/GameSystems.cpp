#include "GameSystems.h"

#define SET_VAR(var, data) var = (data); \
if(var) \
	LogF(LP_DATA, "{}: {}", #var, (void*)var); \
else \
	LogF(LP_ERROR, "{}: {}", #var, (void*)var);

void GameSystems::FindGameSystems() {
	Log(LP_INFO, "GAME SYSTEM POINTERS:");

	// Also in Source2Client::Init(), right after "g_GameEventManager.Init()":
	// mov rcx, [XXXXXXXXX]
	SET_VAR(GameEventManagerPtr, Address(Interfaces::Client->GetVFunc(13).ptr).Offset(0x3E).GetAbsoluteAddress(3));

	// CSource2Client::NotifyClientSignon
	SET_VAR(RichPresence, Address(Interfaces::Client->GetVFunc(47).ptr)
		.Offset(0x23)
		.GetAbsoluteAddress(1)
		.GetAbsoluteAddress(3));

	// CSource2Client::NotifyClientSignon, first call
	SET_VAR(GCClientSystem, Address(Interfaces::Client->GetVFunc(47).ptr)
		.Offset(0x8)
		.GetAbsoluteAddress(1)
		.GetAbsoluteAddress(3));

	//xref: "activategameui", first lea rax, [XXXXXXXXX]
	//console command ^
	SET_VAR(GameUI, SigScan::Find("E8 ? ? ? ? 48 8B C8 E8 ? ? ? ? 48 8B 3D", "client.dll")
		.GetAbsoluteAddress(1)
		.GetAbsoluteAddress(3));

	// xref "Spews a list of all client-side projectiles", above it is lea rax, [XXXXXXXX]
	// I want to die looking at that sig
	SET_VAR(ProjectileManagerPtr,
		SigScan::Find("48 8B 0D ? ? ? ? 48 85 C9 0F 85 ? ? ? ? C3 CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC 48 8D 0D", "client.dll")
		.GetAbsoluteAddress(3, 7));

	SET_VAR(GameRulesPtr, Address(CDOTAGameRules::GetGameTimeFunc).Offset(0xF).GetAbsoluteAddress(3, 7));

	// xref: "No player resource\n"
	SET_VAR(PlayerResourcePtr, SigScan::Find("40 57 48 83 EC 70 48 8B 3D", "client.dll")
		.Offset(6)
		.GetAbsoluteAddress(3, 7));

	// xref: "flOffset"
	// lea rcx, [XXXXXXXXX] above it is GetParticleManager()
	SET_VAR(ParticleManagerPtr, SigScan::Find("E8 ? ? ? ? 8B 14 9F", "client.dll")
		.GetAbsoluteAddress(1, 5)
		.GetAbsoluteAddress(3, 7));
}