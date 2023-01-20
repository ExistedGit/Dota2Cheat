#pragma once
#include "Globals.h"
#include "Interfaces.h"
#include "Wrappers.h"

extern bool IsInMatch;
extern DotaPlayer* localPlayer;
extern BaseNpc* assignedHero;



inline void EnteredMatch() {

	Globals::GameRules = *Globals::GameRulesPtr;
	if ((Globals::GameRules->GetGameState() == GameState::DOTA_GAMERULES_PREGAME ||
		Globals::GameRules->GetGameState() == GameState::DOTA_GAMERULES_GAME_IN_PROGRESS)) {
		localPlayer = (DotaPlayer*)Interfaces::Entity->GetBaseEntity(Interfaces::Engine->GetLocalPlayerSlot() + 1);
		assignedHero = (BaseNpc*)Interfaces::Entity->GetBaseEntity(ENTID_FROM_HANDLE(localPlayer->GetAssignedHeroHandle()));
		std::cout << std::hex << "Assigned Hero: " << assignedHero << " " << assignedHero->GetUnitName() << '\n';
		IsInMatch = true;
	}
}
inline void LeftMatch() {
	Globals::GameRules = nullptr;
	localPlayer = nullptr;
	assignedHero = nullptr;
	IsInMatch = false;
}
inline void CheckMatchState() {
	if (Interfaces::Engine->IsInGame()) {
		if (!IsInMatch)
			EnteredMatch();
	}
	else if (IsInMatch) LeftMatch();
}