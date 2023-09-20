#pragma once
#include "../pch.h"
#include "../MatchStateHandling.h"
#include "../../Modules/Hacks/ShakerAttackAnimFix.h"
#include "../../Modules/UI/Indicators/KillIndicator.h"
#include "../../Modules/UI/AbilityESP.h"

class dota_player_update_assigned_hero_l : public IGameEventListener2 {
public:
	void FireGameEvent(CGameEvent* ev) override {
		if (!ctx.localPlayer)
			return;
		static auto hash = GetHash("playerid");
		int playerid = ev->GetInt(&hash, -2);
		if (playerid != ctx.localPlayer->GetPlayerID())
			return;

		MatchStateManager.OnUpdatedAssignedHero();
	}
};