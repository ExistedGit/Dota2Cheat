#pragma once
#include <cstdio>
#include <iostream>
#include "CheatSDK/Hooking.h"

#include "DebugFunctions.h"

#include "Utils/Drawing.h"

#include "CheatSDK/include.h"
#include "CheatSDK/MatchStateHandling.h"

#include "Hooks/InvalidateUEF.h"
#include "Modules/Utility/CVarSpoofer.h"
#include "SDK/Interfaces/GC/CEconWearable.h"
#include "UI/Pages/MainMenu.h"
#include "UI/Pages/AutoPickSelectionGrid.h"
#include "CheatSDK/Systems/CheatManager.h"

Vector Vector::Zero = Vector(0, 0, 0);

uintptr_t WINAPI HackThread(HMODULE hModule) {
	constexpr bool useChangerCode = false;
	// Initialize MinHook.
	if (MH_Initialize() != MH_OK)
		FreeLibraryAndExitThread(hModule, 0);

	Modules::SkinChanger.DeleteSOCacheFiles();

	d2c.FindCheatFolder();

	Log(LP_INFO, "Loading assets...");
	auto iconLoadThread = std::async(std::launch::async, Pages::AutoPickHeroGrid::InitList);

	Log(LP_INFO, "Initializing cheat...");
	d2c.Initialize(hModule);

	iconLoadThread.wait();

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
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH: {
		if (HANDLE thread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, 0); thread)
			CloseHandle(thread);
		break;
	}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}