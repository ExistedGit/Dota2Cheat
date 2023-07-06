#pragma once
#include "../../SDK/pch.h"
#include "../../CheatSDK/include.h"
#include <queue>

namespace Modules {
	inline class M_ModifierESP {
		std::map<CDOTABaseNPC_Hero*, std::priority_queue<CDOTAModifier*>> HeroModifiers;
	public:
		enum ModifierPriority {
			Aura,
			NoExpire, // buffs that don't expire, like SF's soul stacks or Lion's finger stacks 
			Debuff,
			Buff,
			Count
		};
		void OnModifierCreated(CDOTAModifier* buff) {
			auto owner = (CDOTABaseNPC_Hero*)buff->GetOwner();

			if (!ctx.heroes.contains(owner) && owner->IsSameTeam(ctx.localHero))
				return;

			ModifierPriority insertPriority = BUFF;
			if (!buff->IsSameTeam(ctx.localHero))
				insertPriority = DEBUFF;


			HeroModifiers[owner];
		}
		void DrawESP() {

		}
		void UpdateHeroData() {
			
		}
	} ModifierESP{};
}