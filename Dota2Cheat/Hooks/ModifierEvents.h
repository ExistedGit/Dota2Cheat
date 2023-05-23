#pragma once
#include "../SDK/pch.h"
#include "../CheatSDK/include.h"

#include "../Modules/Hacks/EnemySpellHighlight.h"
#include "../Modules/Hacks/TargetedSpellHighlighter.h"
#include "../Modules/Hacks/LinearProjectileWarner.h"
#include "../Modules/Hacks/TrueSightESP.h"

namespace Hooks {
	inline Signatures::OnRemoveModifierFn oOnRemoveModifier = nullptr;
	inline Signatures::OnAddModifierFn oOnAddModifier = nullptr;

	inline void CacheModifier(CDOTAModifier* modifier) {
		auto owner = (CDOTABaseNPC_Hero*)modifier->GetOwner();
		if (!ctx.heroes.contains(owner))
			return;

		HeroData[owner].Modifiers[modifier->GetName()] = modifier;
	}
	inline void UncacheModifier(CDOTAModifier* modifier) {
		auto owner = (CDOTABaseNPC_Hero*)modifier->GetOwner();
		if (!ctx.heroes.contains(owner))
			return;

		HeroData[owner].Modifiers.erase(modifier->GetName());
	}

	void CacheIfItemModifier(CDOTAModifier* modifier);
	void hkOnAddModifier(CDOTAModifier* modifier, int unk);
	void hkOnRemoveModifier(CDOTAModifier* modifier);
}