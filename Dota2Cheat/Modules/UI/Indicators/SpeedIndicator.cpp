#include "SpeedIndicator.h"

void Modules::M_SpeedIndicator::OnFrame()
{
	if (!Config::Indicators::Speed)
		return;
	
	MTM_LOCK;

	for (auto& [idx, data] : renderData) {
		auto hero = CEntSys::Get()->GetEntity<CHero>(idx);

		data.drawable = !hero->IsSameTeam(ctx.localHero)
			&& hero->IsTargetable()
			&& !hero->IsIllusion()
			&& IsEntityOnScreen(hero);

		if (!data.drawable) continue;

		data.pos = HeroData[hero].HealthbarW2S;
		int enemySpeed = hero->GetIdealSpeed(),
			mySpeed = ctx.localHero->GetIdealSpeed();

		if (mySpeed > enemySpeed)
			data.relativeSpeed = 1;
		else if (mySpeed < enemySpeed)
			data.relativeSpeed = -1;
		else 
			data.relativeSpeed = 0;
	}
}
