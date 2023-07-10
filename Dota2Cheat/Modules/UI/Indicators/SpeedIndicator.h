#pragma once
#include "../../../SDK/pch.h"
#include "../../../CheatSDK/include.h"
#include "../../../Utils/Drawing.h"

namespace Modules {
	// Created together with Wolf49406
	inline class M_SpeedIndicator {
		bool DrawCondition(const EntityWrapper& wrap) {
			CDOTABaseNPC_Hero* hero = wrap;
			return !hero->IsSameTeam(ctx.localHero)
				&& hero->IsTargetable()
				&& !hero->IsIllusion()
				&& IsEntityOnScreen(hero);
		}
	public:
		void Draw();
	} SpeedIndicator;
}