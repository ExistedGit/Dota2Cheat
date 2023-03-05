#pragma once
#include "../SDK/include.h"
#include "../Config.h"

//extern DotaPlayer* ctx.localPlayer;

// Uses the wand if the health is at N% of max, where N is hpPercentTreshold(values are percents, like 10 or 20)
inline void AutoUseWandCheck(CDOTABaseNPC* hero, int hpPercentTreshold, int minCharges) {
	if (!Config::AutoWandEnabled)
		return;

	if ((float)hero->GetHealth() / hero->GetMaxHealth() <= hpPercentTreshold / 100.0f) {
		auto wand = hero->FindItemBySubstring("item_magic");
		if (wand.handle == -1)
			return;
		
		//player is for ordering the cast
		auto wandEnt = wand.GetEnt();
		if (wandEnt->GetCooldown() != 0 || wandEnt->GetCharges() < minCharges)
			return;
		ctx.localPlayer->CastNoTarget(wand.handle, hero);
	}
}

inline void AutoUseFaerieFireCheck(CDOTABaseNPC* hero, int hpPercentTreshold) {
	if (!Config::AutoWandEnabled)
		return;

	if ((float)hero->GetHealth() / hero->GetMaxHealth() <= hpPercentTreshold / 100.0f) {
		auto faerieFire = hero->FindItemBySubstring("faerie");
		if (faerieFire.handle == -1)
			return;

		//player is for ordering the cast
		auto itemEnt = faerieFire.GetEnt();
		if (itemEnt->GetCooldown() != 0)
			return;
		ctx.localPlayer->CastNoTarget(faerieFire.handle, hero);
	}
}