#include "GameSystems.h"

void GameSystems::FindGameSystems() {
	std::cout << "[GLOBAL POINTERS]\n";

	SigScanContext ssctx{ ctx.CurProcHandle, ctx.CurProcId };

	// In Source2Client::Init(), right after "g_GameEventManager.Init()":
	// mov rcx, [XXXXXXXXX]
	GameEventManagerPtr = Address(Interfaces::Client->GetVFunc(3).ptr).GetAbsoluteAddress(
		0x106,
		3,
		7);

	std::cout << "GameEventManagerPtr: " << GameEventManagerPtr << '\n';


	// xref "Spews a list of all client-side projectiles", above it is lea rax, [XXXXXXXX]
	// right click -> Find references to -> Address: XXXXXXXX
	auto addr = ssctx.Scan("40 55 53 41 57 48 8D 6C 24 D0 48 81 EC 30 01 00 00 48 89 BC 24 58 01 00 00", L"client.dll");
	ProjectileManagerPtr = addr.GetAbsoluteAddress(0x95e, 3, 7);
	std::cout << "ProjectileManagerPtr: " << ProjectileManagerPtr << '\n';


	GameRulesPtr = Address(CDOTAGameRules::GetGameTimeFunc).GetAbsoluteAddress(0xF, 3, 7);
	std::cout << "GameRulesPtr: " << GameRulesPtr << '\n';

	addr = ssctx.Scan("48 8B ? ? ? ? ? 48 85 C9 0F 85 ? ? ? ? B8 FF FF FF FF C3", L"client.dll");
	PlayerResourcePtr = addr.GetAbsoluteAddress(0, 3, 7);
	std::cout << "PlayerResourcePtr: " << PlayerResourcePtr << '\n';


	addr = ssctx.Scan("7E 0B 41 8D 42 F1 A9 FB FF FF FF 75 0B 41 8B C9 E8 ? ? ? ? 48 8B D8 E8", L"client.dll");
	addr = addr.GetAbsoluteAddress(0x18, 1, 5);
	addr = addr.GetAbsoluteAddress(0, 3, 7);
	ParticleManagerPtr = addr;
	std::cout << "DPMP: " << ParticleManagerPtr << '\n';
}

void GameSystems::LogGameSystems() {
	std::cout << "[GLOBALS]\n";
	std::cout << "GameRules: " << GameRules << '\n';
	std::cout << "Projectile Manager:" << ProjectileManager << '\n';
	std::cout << "Particle Manager: " << ParticleManager << ' ' << ParticleManager->GetVFunc(9).ptr << "\n";
}
