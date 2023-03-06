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
		if (!wand)
			return;
		
		if (wand->GetCooldown() != 0 || wand->GetCharges() < minCharges)
			return;

		ctx.localPlayer->CastNoTarget(wand->GetIdentity()->entHandle, hero);
	}
}

inline void AutoUseFaerieFireCheck(CDOTABaseNPC* hero, int hpPercentTreshold) {
	if (!Config::AutoWandEnabled)
		return;

	if ((float)hero->GetHealth() / hero->GetMaxHealth() <= hpPercentTreshold / 100.0f) {
		auto faerieFire = hero->FindItemBySubstring("faerie");
		if (!faerieFire)
			return;

		ctx.localPlayer->CastNoTarget(faerieFire->GetIdentity()->entHandle, hero);
	}
}