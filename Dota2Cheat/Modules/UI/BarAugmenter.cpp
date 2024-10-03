#include "BarAugmenter.h"

bool Modules::M_BarAugmenter::CanDraw(CHero* hero) {
	return hero
		&& hero->GetIdentity()
		&& !hero->GetIdentity()->IsDormant()
		&& !hero->IsSameTeam(ctx.localHero)
		&& !hero->IsIllusion()
		&& hero != ctx.localHero
		&& hero->GetLifeState() == 0
		&& IsEntityOnScreen(hero);
}

