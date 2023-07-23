#pragma once
#include "../../pch.h"
#include "../../CheatSDK/include.h"

namespace Modules {
	inline class M_ModifierESP {
	public:
		enum ModifierPriority {
			AuraDebuff,
			AuraBuff,
			NoExpire, // buffs that don't expire, like SF's soul stacks or Lion's finger stacks 
			Debuff,
			Buff,
			Count
		};
		std::map<CDOTABaseNPC*, std::map<ModifierPriority, std::set<CDOTAModifier*>>> HeroModifiers;

		void OnModifierCreated(CDOTAModifier* buff) {
			using enum ModifierPriority;

			auto owner = (CDOTABaseNPC_Hero*)buff->GetOwner();

			if (!EntityList.IsHero(owner) && owner->IsSameTeam(ctx.localHero) || owner->IsIllusion())
				return;

			ModifierPriority priority = buff->IsAura() ? AuraBuff : Buff;
			if (!buff->IsSameTeam(ctx.localHero))
				priority = buff->IsAura() ? AuraDebuff : Debuff;
			
			HeroModifiers[owner][priority].insert(buff);
		}

		void OnModifierRemoved(CDOTAModifier* buff) {
			for (auto& [p, s] : HeroModifiers[buff->GetOwner()])
				s.erase(buff);
		}

		void DrawESP() {

		}

		void UpdateHeroData() {

		}
	} ModifierESP{};
}