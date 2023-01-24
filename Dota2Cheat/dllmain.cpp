// dllmain.cpp : Определяет точку входа для приложения DLL.
#pragma once
#include "pch.h"
#include <cstdio>
#include <iostream>
#include "vtabler.h"
#include "Wrappers.h"
#include "SDK/color.h"
#include "AutoUseMagicWand.h"
#include "AutoUseMidas.h"
#include "AutoBuyTome.h"
#include "Globals.h"
#include "MatchStateHandling.h"
#include "Hooks.h"
#pragma region Global variables

bool IsInMatch = false;
Vector3 Vector3::Zero = Vector3(0, 0, 0);
std::map<ConVar*, int> CVarSystem::CVar = {};

DotaPlayer* localPlayer;
BaseNpc* assignedHero;
std::vector<DotaPlayer*> players{};

HANDLE CurProcHandle;
int CurProcId;
#pragma endregion




//int GetLocalPlayerSlot() {
//	int idx = 0;
//	getvfunc(Interfaces::Engine, 20)(Interfaces::Engine, &idx, 0, 0);
//	return idx;
//}

void gotoxy(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}


//std::vector<Color> rainbow = {
//	Color(255,0,0),//red
//	Color(255,255,0),
//	Color(0,255,0),//green
//	Color(0,255,255),
//	Color(0,0,255),//blue
//	Color(255,0,255)
//};
//int rainbowIndex = 0;



uintptr_t WINAPI HackThread(HMODULE hModule) {
	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);
	{
		CurProcId = GetCurrentProcessId();
		CurProcHandle = OpenProcess(PROCESS_ALL_ACCESS, TRUE, CurProcId);
	}


	std::cout << "works!" << std::endl;
	Interfaces::InitInterfaces();
	Interfaces::LogInterfaces();
	//Interfaces::CVar->DumpConVarsToFile("H:\\SchemaDump\\convars.txt");
	Interfaces::CVar->DumpConVarsToMap();

	Schema::SchemaDumpToMap("client.dll", "C_DOTA_BaseNPC_Hero");
	Schema::SchemaDumpToMap("client.dll", "C_DOTAPlayerController");
	Schema::SchemaDumpToMap("client.dll", "C_DOTA_UnitInventory");

	Signatures::InitSignatures();
	Signatures::LogSignatures();

	Globals::InitGlobals();
	//Globals::LogGlobals();
	const bool logEntities = false;
	bool playerEntFound = false;

	if (logEntities) {
		//LogEntities();

		VMTs::Entity = std::unique_ptr<VMT>(new VMT(Interfaces::Entity));
		VMTs::Entity->HookVM(Hooks::OnAddEntity, 14);
		VMTs::Entity->ApplyVMT();
	}

	Log("CVars found!");
	//bool spamBuy = false;
	while (!GetAsyncKeyState(VK_INSERT)) {
		Sleep(10);
		CheckMatchState();
	}
	//VMTs::Entity->ReleaseVMT();
	//VMTs::Panorama2->ReleaseVMT();
	//VMTs::Entity = nullptr;
	Schema::Netvars.clear();
	if (f) fclose(f);
	FreeConsole();
	FreeLibraryAndExitThread(hModule, 0);
}
BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH: {

		const HANDLE thread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, 0);
		if (thread) CloseHandle(thread);

		break;
	}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

