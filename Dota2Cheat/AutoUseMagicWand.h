#pragma once
#include "Wrappers.h"
#include "Config.h"

//extern DotaPlayer* ctx.localPlayer;

// Uses the wand if the health is at N% of max, where N is hpPercentTreshold(values are percents, like 10 or 20)
// if you don't specify the hero it will check your main hero
inline void AutoUseWandCheck(BaseNpc* hero, int hpPercentTreshold, int minCharges) {
	if (!Config::AutoWandEnabled)
		return;

	if ((float)hero->GetHealth() / hero->GetMaxHealth() <= hpPercentTreshold / 100.0f) {
		auto wand = hero->FindItemBySubstring("item_magic");
		if (wand.handle == -1)
			return;
		
		//player is for ordering the cast
		auto wandEnt = wand.GetAs<BaseAbility>();
		if (wandEnt->GetCooldown() != 0 || wandEnt->GetCharges() < minCharges)
			return;
		ctx.localPlayer->CastNoTarget(wand.handle, hero);
	}
}
inline void AutoUseFaerieFireCheck(BaseNpc* hero, int hpPercentTreshold) {
	if (!Config::AutoWandEnabled)
		return;

	if ((float)hero->GetHealth() / hero->GetMaxHealth() <= hpPercentTreshold / 100.0f) {
		auto faerieFire = hero->FindItemBySubstring("faerie");
		if (faerieFire.handle == -1)
			return;

		//player is for ordering the cast
		auto itemEnt = faerieFire.GetAs<BaseAbility>();
		if (itemEnt->GetCooldown() != 0)
			return;
		ctx.localPlayer->CastNoTarget(faerieFire.handle, hero);
	}
}