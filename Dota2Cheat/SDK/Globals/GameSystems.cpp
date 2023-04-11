#include "GameSystems.h"

void GameSystems::FindGameSystems() {
	std::cout << "[GLOBAL POINTERS]\n";

	SigScanContext ssctx{ ctx.CurProcHandle, ctx.CurProcId };

	// In Source2Client::Init(), right after "g_GameEventManager.Init()":
	// mov rcx, [XXXXXXXXX]
	GameEventManagerPtr = Address(Interfaces::Client->GetVFunc(3).ptr).Offset(0x106).GetAbsoluteAddress(3, 7);
	std::cout << "GameEventManagerPtr: " << GameEventManagerPtr << '\n';

	// CSource2Client::NotifySignon
	RichPresence = Address(Interfaces::Client->GetVFunc(47).ptr)
		.Offset(0x23)
		.GetAbsoluteAddress(1)
		.GetAbsoluteAddress(3);

	//xref: "Failed to get custom game %llu timestamp/CRC!\n"
	GCClientSystem =
		ssctx.Scan("E8 ? ? ? ? 48 8B D7 48 8B CB E8 ? ? ? ? 48 8D 8B", L"client.dll")
		.GetAbsoluteAddress(1)
		.Offset(0x277)
		.GetAbsoluteAddress(3);

	//xref: "activategameui", first lea rax, [XXXXXXXXX]
	//console command ^
	GameUI = ssctx.Scan("E8 ? ? ? ? 48 8B C8 E8 ? ? ? ? 48 8B 3D", L"client.dll")
		.GetAbsoluteAddress(1)
		.GetAbsoluteAddress(3);

	// xref "Spews a list of all client-side projectiles", above it is lea rax, [XXXXXXXX]
	// right click -> Find references to -> Address: XXXXXXXX
	auto addr = ssctx.Scan("40 55 53 41 57 48 8D 6C 24 D0 48 81 EC 30 01 00 00 48 89 BC 24 58 01 00 00", L"client.dll");
	ProjectileManagerPtr = addr.Offset(0x95e).GetAbsoluteAddress(3, 7);
	std::cout << "ProjectileManagerPtr: " << ProjectileManagerPtr << '\n';


	GameRulesPtr = Address(CDOTAGameRules::GetGameTimeFunc).Offset(0xF).GetAbsoluteAddress(3, 7);
	std::cout << "GameRulesPtr: " << GameRulesPtr << '\n';

	PlayerResourcePtr = ssctx.Scan("48 8B ? ? ? ? ? 48 85 C9 0F 85 ? ? ? ? B8 FF FF FF FF C3", L"client.dll")
		.GetAbsoluteAddress(3, 7);
	std::cout << "PlayerResourcePtr: " << PlayerResourcePtr << '\n';

	ParticleManagerPtr = ssctx.Scan("7E 0B 41 8D 42 F1 A9 FB FF FF FF 75 0B 41 8B C9 E8 ? ? ? ? 48 8B D8 E8", L"client.dll")
		.Offset(0x18)
		.GetAbsoluteAddress(1, 5)
		.GetAbsoluteAddress(3, 7);

	
	std::cout << "DPMP: " << ParticleManagerPtr << '\n';
}

void GameSystems::LogGameSystems() {
	std::cout << "[GLOBALS]\n";
	std::cout << "GameRules: " << GameRules << '\n';
	std::cout << "Projectile Manager:" << ProjectileManager << '\n';
	std::cout << "CDOTARichPresence:" << RichPresence << '\n';
	std::cout << "Particle Manager: " << ParticleManager << ' ' << ParticleManager->GetVFunc(VTableIndexes::CDOTAParticleManager::CreateParticle).ptr << "\n";
}
