#pragma once
#include <cstdio>
#include <iostream>
#include "CheatSDK/Hooking.h"

#include "DebugFunctions.h"

#include "Utils/Drawing.h"

#include "CheatSDK/include.h"
#include "CheatSDK/MatchStateHandling.h"

#include "UI/Pages/MainMenu.h"
#include "UI/Pages/AutoPickSelectionGrid.h"

uintptr_t WINAPI HackThread(HMODULE hModule) {
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
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
		if (HANDLE thread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, 0); thread)
			CloseHandle(thread);

	return TRUE;
}