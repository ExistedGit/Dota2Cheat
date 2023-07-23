#pragma once
#include "CheatSDK/Hooking.h"

#include "DebugFunctions.h"

#include "Utils/Drawing.h"

#include "CheatSDK/include.h"
#include "CheatSDK/MatchStateHandling.h"

#include "UI/Pages/MainMenu.h"
#include "UI/Pages/AutoPickSelectionGrid.h"

static void* oSpawn{};
void* hkSpawn(CDOTABaseNPC* hero, KeyValues* kv) {
	auto w = hero->Member<CUtlVector<CHandle<CEconWearable>>>(Netvars::C_BaseCombatCharacter::m_hMyWearables);
	Log(LP_NONE, "WEARABLES:");
	for (int i = 0; i < w.m_Size; i++) {
		LogF(LP_NONE, "{} | {}", i, (void*)w[i].Entity());
	}

	static Function init = Memory::Scan("E8 ? ? ? ? 8B C6 3B F3", "client.dll").GetAbsoluteAddress(1);

	auto item = w[0]->GetAttributeManager()->GetItem();
	init(item, Modules::SkinChanger.FakeItems.begin()->second, 0);

	//Log(LP_NONE, "KEYVALUES:");
	//for (auto key = kv->GetFirstSubKey(); key; key = key->GetNextKey()) {
	//	LogF(LP_NONE, "{} | {}", (int)key->GetDataType(), key->GetName());
	//};

	return ((decltype(&hkSpawn))oSpawn)(hero, kv);
}

uintptr_t WINAPI HackThread(HMODULE hModule) {
	// Initialize MinHook.
	if (MH_Initialize() != MH_OK)
		FreeLibraryAndExitThread(hModule, 0);

	Modules::SkinChanger.DeleteSOCacheFiles();

	//{
	//	auto Spawn = Memory::Scan("E8 ? ? ? ? 44 0F B6 0D ? ? ? ? 48 8B CB", "client.dll").GetAbsoluteAddress(1);
	//	HOOKFUNC(Spawn);
	//}

	Log(LP_INFO, "Initializing cheat...");
	d2c.Initialize(hModule);

	MatchStateManager.CheckForOngoingGame();

	while (!d2c.shouldUnload)
		Sleep(10);

	d2c.Unload();
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
		if (HANDLE thread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, 0); thread)
			CloseHandle(thread);

	return TRUE;
}