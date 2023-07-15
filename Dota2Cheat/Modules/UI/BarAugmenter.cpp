#include "BarAugmenter.h"

bool Modules::BarAugmenter::CanDraw(CDOTABaseNPC_Hero* hero) {
	return IsValidReadPtr(hero)
		&& IsValidReadPtr(hero->GetIdentity())
		&& !hero->GetIdentity()->IsDormant()
		&& !hero->IsSameTeam(ctx.localHero)
		&& !hero->IsIllusion()
		&& hero != ctx.localHero
		&& hero->GetLifeState() == 0
		&& IsEntityOnScreen(hero);
}

