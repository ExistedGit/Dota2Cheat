#pragma once
#include "../SDK/pch.h"
#include "../../Hooks/ModifierEvents.h"
#include "../../Modules/Hacks/ShakerAttackAnimFix.h"

class dota_player_update_assigned_hero_l : public IGameEventListener2 {
public:
	void DESTROY() override {

	}
	void FireGameEvent(CGameEvent* ev) override {
		if (!ctx.localPlayer)
			return;

		int playerid = ev->GetInt("playerid", -2);
		if (playerid != ctx.localPlayer->GetPlayerID())
			return;

		CDOTABaseNPC_Hero* assignedHero = ctx.localPlayer->GetAssignedHeroHandle();

		ctx.localHero = assignedHero;

		LogF(LP_INFO, "Changed hero: \n\tEntity: {}\n\tName: ", (void*)assignedHero, assignedHero->GetUnitName());

		ctx.lua["localHero"] = ctx.localHero;

		Modules::AbilityESP.SubscribeHeroes();
		Modules::KillIndicator.Init();
		Modules::ShakerAttackAnimFix.SubscribeEntity(ctx.localHero);

		//for (auto& modifier : ctx.localHero->GetModifierManager()->GetModifierList()) {
		//	Hooks::CacheIfItemModifier(modifier); // for registering items on reinjection
		//	Hooks::CacheModifier(modifier);
		//}

		Lua::CallModuleFunc("OnUpdatedAssignedHero");
	}
};