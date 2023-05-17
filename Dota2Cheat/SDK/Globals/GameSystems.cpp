#include "GameSystems.h"
#include "Interfaces.h"

#define SET_VAR(var, data) var = (decltype(var))(data); \
if(var) \
	LogF(LP_DATA, "{}: {}", #var, (void*)var); \
else \
	LogF(LP_ERROR, "{}: {}", #var, (void*)var);

void GameSystems::InitMinimapRenderer() {
	while (!DotaHud->FindChildWithIdTraverse("minimap")) {};
	auto minimap = DotaHud->FindChildWithIdTraverse("minimap");
	
	SET_VAR(MinimapRenderer, minimap->GetPanel2D()->Member<CDOTAPanoramaMinimapRenderer*>(0x20));
}

void GameSystems::FindGameSystems() {
	Log(LP_INFO, "GAME SYSTEM POINTERS:");
	SignatureDB::ParseSignatures(NamedSystems);

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
	//SET_VAR(GameUI, Memory::Scan("E8 ? ? ? ? 48 8B C8 E8 ? ? ? ? 48 8B 3D", "client.dll")
	//	.GetAbsoluteAddress(1)
	//	.GetAbsoluteAddress(3));

	SET_VAR(GameRulesPtr, Address(CDOTAGameRules::GetGameTimeFunc).Offset(0xF).GetAbsoluteAddress(3, 7));

	// xref: "No player resource\n"
	//SET_VAR(PlayerResourcePtr, Memory::Scan("40 57 48 83 EC 70 48 8B 3D", "client.dll")
	//	.Offset(6)
	//	.GetAbsoluteAddress(3, 7));

	// xref: "flOffset"
	// lea rcx, [XXXXXXXXX] above it is GetParticleManager()
	//SET_VAR(ParticleManagerPtr, Memory::Scan("E8 ? ? ? ? 8B 14 9F", "client.dll")
	//	.GetAbsoluteAddress(1, 5)
	//	.GetAbsoluteAddress(3, 7));
}