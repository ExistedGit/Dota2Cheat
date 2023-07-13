#pragma once
#include "../../pch.h"
#include "../../CheatSDK/include.h"

namespace Modules {
	//inline class M_ModifierESP {
	//	std::map<CDOTABaseNPC_Hero*, std::vector<>> HeroModifiers;
	//public:
	//	enum class ModifierPriority {
	//		Aura,
	//		NoExpire, // buffs that don't expire, like SF's soul stacks or Lion's finger stacks 
	//		Debuff,
	//		Buff,
	//		Count
	//	};
	//	struct BuffData{
	//		ModifierPriri
	//	};
	//	void OnModifierCreated(CDOTAModifier* buff) {
	//		using enum ModifierPriority;

	//		auto owner = (CDOTABaseNPC_Hero*)buff->GetOwner();

	//		if (!ctx.heroes.contains(owner) && owner->IsSameTeam(ctx.localHero))
	//			return;
	//		auto insertPriority = Buff;
	//		if (!buff->IsSameTeam(ctx.localHero))
	//			insertPriority = Debuff;

	//		HeroModifiers[owner].

	//		HeroModifiers[owner];
	//	}
	//	void DrawESP() {

	//	}
	//	void UpdateHeroData() {

	//	}
	//} ModifierESP{};
}