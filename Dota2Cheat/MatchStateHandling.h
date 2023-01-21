#pragma once
#include "Globals.h"
#include "Interfaces.h"
#include "Wrappers.h"
#include "AutoBuyTome.h"

extern bool IsInMatch;



inline void EnteredMatch() {
	Globals::GameRules = *Globals::GameRulesPtr;
	GameState gameState = Globals::GameRules->GetGameState();
	if ((gameState == GameState::DOTA_GAMERULES_PREGAME ||
		gameState == GameState::DOTA_GAMERULES_GAME_IN_PROGRESS)) {
		localPlayer = (DotaPlayer*)Interfaces::Entity->GetBaseEntity(Interfaces::Engine->GetLocalPlayerSlot() + 1);
		if (localPlayer == nullptr)
			return;
		assignedHero = (BaseNpc*)Interfaces::Entity->GetBaseEntity(ENTID_FROM_HANDLE(localPlayer->GetAssignedHeroHandle()));
		if (assignedHero == nullptr)
			return;
		Hacks::AutoBuyTomeInit();
		std::cout << std::hex << "Assigned Hero: " << assignedHero << " " << assignedHero->GetUnitName() << '\n';
		IsInMatch = true;
		std::cout << "ENTERED MATCH\n";
	}
}
inline void LeftMatch() {
	Globals::GameRules = nullptr;
	localPlayer = nullptr;
	assignedHero = nullptr;
	IsInMatch = false;
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