#pragma once
#include "../SDK/pch.h"
#include "../Modules/Hacks/EnemySpellHighlight.h"
#include "../Modules/Hacks/TargetedSpellHighlighter.h"
#include "../Modules/Hacks/LinearProjectileWarner.h"
#include "../Modules/Hacks/TrueSightESP.h"

namespace Hooks {
	inline Signatures::OnRemoveModifierFn oOnRemoveModifier = nullptr;
	inline Signatures::OnAddModifierFn oOnAddModifier = nullptr;

	inline void CacheModifier(CDOTAModifier* modifier) {
		if (!ctx.heroes.contains((CDOTABaseNPC_Hero*)modifier->GetOwner()))
			return;

		HeroData[modifier->GetOwner()].Modifiers[modifier->GetName()] = modifier;
	}
	inline void UncacheModifier(CDOTAModifier* modifier) {
		if (!ctx.heroes.contains((CDOTABaseNPC_Hero*)modifier->GetOwner()))
			return;

		HeroData[modifier->GetOwner()].Modifiers.erase(modifier->GetName());
	}

	void CacheIfItemModifier(CDOTAModifier* modifier);
	void hkOnAddModifier(CDOTAModifier* modifier, int unk);
	void hkOnRemoveModifier(CDOTAModifier* modifier);
}