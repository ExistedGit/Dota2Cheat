#pragma once
#include "../pch.h"
#include "../CheatSDK/include.h"

#include "../Modules/Hacks/EnemySpellHighlight.h"
#include "../Modules/Hacks/TargetedSpellHighlighter.h"
#include "../Modules/Hacks/LinearProjectileWarner.h"
#include "../Modules/Hacks/TrueSightESP.h"

namespace Hooks {
	inline void* oOnRemoveModifier = nullptr;
	inline void* oOnAddModifier = nullptr;

	inline void CacheModifier(CDOTAModifier* modifier) {
		auto owner = (CDOTABaseNPC_Hero*)modifier->GetOwner();
		if (!EntityList.IsHero(owner))
			return;

		HeroData[owner].Modifiers[modifier->GetName()] = modifier;
	}
	inline void UncacheModifier(CDOTAModifier* modifier) {
		auto owner = (CDOTABaseNPC_Hero*)modifier->GetOwner();
		if (!EntityList.IsHero(owner))
			return;

		HeroData[owner].Modifiers.erase(modifier->GetName());
	}

	void CacheIfItemModifier(CDOTAModifier* modifier);
	void hkOnAddModifier(CDOTAModifier* modifier, int unk);
	void hkOnRemoveModifier(CDOTAModifier* modifier);
}