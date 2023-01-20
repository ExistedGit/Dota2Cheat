// dllmain.cpp : Определяет точку входа для приложения DLL.
#pragma once
#include "pch.h"
#include <cstdio>
#include <iostream>
#include "vtabler.h"
#include "Wrappers.h"
#include "SDK/color.h"
#include "AutoUseMagicWand.h"
#include "Globals.h"
#include "AutoUseMidas.h"

Fvector Fvector::Zero = Fvector(0, 0, 0);
std::map<ConVar*, int> CVarSystem::CVar = {};

HANDLE CurProcHandle;
int CurProcId;

namespace VMTs {
	std::unique_ptr<VMT> Engine;
	std::unique_ptr<VMT> Entity;
}

namespace Hooks {
	typedef BaseEntity* (*OnAddEntityFn)(CEntitySystem*, BaseEntity*, ENT_HANDLE);

	BaseEntity* OnAddEntity(CEntitySystem* thisptr, BaseEntity* ent, ENT_HANDLE handle) {
		bool fit = true;
		const char* name = ent->SchemaBinding()->binaryName;
		if (fit) {
			int entId = ((uint16_t)(handle) & 0xfff);
			//CDotaBaseNPC* npc = (CDotaBaseNPC*)ent;
			//if (name != nullptr)
			//	std::cout << name << " // " << ent << " || " << std::dec << entId << std::hex << std::endl;
			if (strstr(ent->SchemaBinding()->binaryName, "DOTA_Unit_Hero") != nullptr) {
				std::cout << ent->SchemaBinding()->binaryName << " // " << ent << '\n';
			}
			else if (strstr(ent->SchemaBinding()->binaryName, "C_DOTAPlayer") != nullptr) {
				auto* player = (DotaPlayer*)ent;
				std::cout << "Player // " << player << " || " << entId << std::endl;
			}

		}

		return VMTs::Entity->GetOriginalMethod<OnAddEntityFn>(14)(thisptr, ent, handle);
	};
}

void LogEntities() {
	for (int i = 0; i < Interfaces::Entity->GetHighestEntityIndex(); i++) {
		auto* ent = Interfaces::Entity->GetBaseEntity(i);
		if (ent != nullptr) {
			BaseEntity* wrapper = (BaseEntity*)ent;
			//std::cout << ent->SchemaBinding() << '\n';
			const char* className = ent->SchemaBinding()->binaryName;
			if (className != nullptr) {
				std::cout << className << " // ";
				std::cout << ent << '\n';
			}
		}
	}
}


int GetLocalPlayerSlot() {
	int idx = 0;
	getvfunc(Interfaces::Engine, 20)(Interfaces::Engine, &idx, 0, 0);
	return idx;
}

void gotoxy(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

DotaPlayer* localPlayer;
BaseNpc* assignedHero;

void LogInvAndAbilities() {
	std::cout << "abilities: " << '\n';
	for (const auto& ability : assignedHero->GetAbilities())
		std::cout << '\t' << ability.name << " " << ENTID_FROM_HANDLE(ability.handle) << "Mana Cost: " << getvfunc(Interfaces::Entity->GetBaseEntity(ENTID_FROM_HANDLE(ability.handle)), 0xc4).ptr << '\n';
	std::cout << "inventory: " << '\n';
	for (const auto& item : assignedHero->GetItems())
		std::cout << '\t' << item.name << " " << ENTID_FROM_HANDLE(item.handle) << '\n';
}
bool TestStringFilters(const char* str, std::vector<const char*> filters) {
	bool result = false;
	for (auto& filter : filters) {
		if (strstr(str, filter)) {
			result = true;
			break;
		}
	}
	return result;
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

void EntityIteration(ENT_HANDLE midas) {
	int illusionCount = 0;
	bool midasUsed = false;

	for (int i = 0; i < Interfaces::Entity->GetHighestEntityIndex(); i++) {
		auto* ent = Interfaces::Entity->GetBaseEntity(i);

		if (ent == nullptr)
			continue;
		const char* className = ent->SchemaBinding()->binaryName;
		if (!midasUsed && midas != -1
			&& className != nullptr
			&& strstr(className, "Creep")) {
			auto creep = (BaseNpc*)ent;
			Fvector posHero = assignedHero->GetPos(), posCreep = creep->GetPos();
			static std::vector<const char*> filters = {
					"ranged",
					"flagbearer",
					"siege",
					"alpha_wolf",
					"centaur_khan",
					"dark_troll_warlord",
					"ursa_warrior",
					"ogre_magi"
			};
			// If the creep is not one of ours, is alive, is within 600 hammer units and its name matches one of the filters
			if (creep->GetTeam() != assignedHero->GetTeam() &&
				creep->GetHealth() > 0 &&
				sqrtf(
					pow(posHero.x - posCreep.x, 2) +
					pow(posHero.y - posCreep.y, 2)
				) <= 600.0f &&
				TestStringFilters(creep->GetUnitName(), filters)) {

				midasUsed = true;
				localPlayer->PrepareOrder(DotaUnitOrder_t::DOTA_UNIT_ORDER_CAST_TARGET, i, &Fvector::Zero, ENTID_FROM_HANDLE(midas), PlayerOrderIssuer_t::DOTA_ORDER_ISSUER_PASSED_UNIT_ONLY);
				break;

			}
		}
		else if (strstr(className, "DOTA_Unit_Hero") != nullptr) {
			auto hero = (BaseNpc*)ent;

			std::cout << std::hex;

			if (hero->BaseNPCHero_IsIllusion() &&
				strstr(className, "CDOTA_Unit_Hero_ArcWarden") == nullptr) {
				illusionCount++;
				if (assignedHero->GetTeam() == hero->GetTeam())
					hero->SetColor(Color(0, 255, 0));
				else
					hero->SetColor(Color(255, 0, 0));
			}
		}

	}
	//rainbowIndex++;
	//rainbowIndex %= rainbow.size();
	//gotoxy(0, 5);
	//std::cout << "Illusions: " << illusionCount;
}

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
	const bool inGameStuff = true;
	bool playerEntFound = false;

	if (logEntities) {
		LogEntities();

		VMTs::Entity = std::unique_ptr<VMT>(new VMT(Interfaces::Entity));
		VMTs::Entity->HookVM(Hooks::OnAddEntity, 14);
		VMTs::Entity->ApplyVMT();
	}


	Log("CVars found!");
	bool spamBuy = false;
	while (!GetAsyncKeyState(VK_INSERT)) {
		Sleep(100);
		bool isInGame = Interfaces::Engine->IsInGame();
		if (!isInGame)
			continue;
		//EntityIteration();

		Globals::GameRules = *Globals::GameRulesPtr;
		//uintptr_t vtable = *((uintptr_t*)(Interfaces::Engine));
		//uintptr_t entry = vtable + sizeof(uintptr_t) * 25;

		//unsigned char IsInGameResult = ((unsigned char(__fastcall*)(void*)) * (uintptr_t*)entry)(Interfaces::Engine);


		if (inGameStuff) {
			if (!playerEntFound && 
				(Globals::GameRules->GetGameState()== GameState::DOTA_GAMERULES_PREGAME ||
					Globals::GameRules->GetGameState() == GameState::DOTA_GAMERULES_GAME_IN_PROGRESS)) {

				localPlayer = (DotaPlayer*)Interfaces::Entity->GetBaseEntity(Interfaces::Engine->GetLocalPlayerSlot() + 1);
				assignedHero = (BaseNpc*)Interfaces::Entity->GetBaseEntity(localPlayer->GetAssignedHeroHandle() & 0x7fff);
				std::cout << std::hex << "assignedHero: " << assignedHero << '\n';
				std::cout << "Local hero: " << assignedHero->GetUnitName() << '\n';
				playerEntFound = true;
				
			}
			if (assignedHero->GetLifeState() == 0) { // if alive
				AutoUseWandCheck(localPlayer, assignedHero);
				AutoUseFaerieFireCheck(localPlayer, assignedHero);
				EntityIteration(AutoUseMidasCheck(assignedHero));
			}

			if (GetAsyncKeyState(VK_NUMPAD7)) {
				//LogInvAndAbilities();
				LogEntities();
			}

			if (GetAsyncKeyState(VK_HOME)) {
				Interfaces::CVar->SetConvars();
			}

			if (spamBuy)
				localPlayer->BuyItem(0x101);
		}

		//if (GetAsyncKeyState(VK_F5)) {
			//for (const auto& item : assignedHero.BaseNPC_GetItems()) {
			//	if (strstr(item.name, "tome") != nullptr)
			//		localPlayer.CastNoTarget(item.handle);
			//}
		//}


	}
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

