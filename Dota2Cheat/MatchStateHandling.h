#pragma once
#include "Globals.h"
#include "Interfaces.h"
#include "Wrappers.h"
#include "Hooks.h"
#include "AutoBuyTome.h"

extern bool IsInMatch;

inline void FillPlayerList() {
	auto vec = Globals::PlayerResource->GetVecTeamPlayerData();
	std::cout << "<PLAYERS>\n";
	for (int i = 0; i < vec.m_Size; i++) {
		//Sleep(800);
		auto idx = vec[i].GetPlayerSlot() + 1;
		if (idx < 1)
			continue;
		auto player = (DotaPlayer*)Interfaces::Entity->GetBaseEntity(idx);
		if (player == nullptr)
			continue;
		auto hero = (BaseNpc*)player->GetAssignedHero();
		//std::cout << idx << " " << player << ' ' << player->GetIdentity() << '\n';

		std::cout << "Player " << std::dec << idx << ": " << player;
		if (hero != nullptr &&
			hero->GetUnitName() != nullptr)
			std::cout << "\n\t" << hero->GetUnitName() << " " << hero;
		std::cout << '\n';
		players.push_back(player);
	}
}

inline void EnteredMatch() {
	Globals::GameRules = *Globals::GameRulesPtr;
	Globals::PlayerResource = *Globals::PlayerResourcePtr;

	GameState gameState = Globals::GameRules->GetGameState();
	if (Globals::GameRules->GetGameTime() >= 2.0f &&
		(gameState == GameState::DOTA_GAMERULES_PREGAME ||
			gameState == GameState::DOTA_GAMERULES_GAME_IN_PROGRESS)) {
		localPlayer = (DotaPlayer*)Interfaces::Entity->GetBaseEntity(Interfaces::Engine->GetLocalPlayerSlot() + 1);
		if (localPlayer == nullptr)
			return;
		assignedHero = (BaseNpc*)Interfaces::Entity->GetBaseEntity(ENTID_FROM_HANDLE(localPlayer->GetAssignedHeroHandle()));
		if (assignedHero == nullptr)
			return;
		Hacks::AutoBuyTomeInit();
		std::cout << std::hex << "Local Player: " << localPlayer << '\n';
		std::cout << std::hex << "Assigned Hero: " << assignedHero << " " << assignedHero->GetUnitName() << '\n';
		IsInMatch = true;
		//FillPlayerList();
		VMTs::Panorama2 = std::unique_ptr<VMT>(new VMT(Interfaces::Panorama2));
		VMTs::Panorama2->HookVM(Hooks::RunFrame, 6);
		VMTs::Panorama2->ApplyVMT();
		std::cout << "ENTERED MATCH\n";
	}
}
inline void LeftMatch() {
	Globals::PlayerResource = nullptr;
	Globals::GameRules = nullptr;
	localPlayer = nullptr;
	assignedHero = nullptr;
	IsInMatch = false;
	players.clear();
	std::cout << "LEFT MATCH\n";
}
inline void CheckMatchState() {
	//std::cout << Interfaces::Engine->IsInGame() << " " << IsInMatch << '\n';
	if (Interfaces::Engine->IsInGame()) {
		if (!IsInMatch)
			EnteredMatch();
	}
	else if (IsInMatch) LeftMatch();
}