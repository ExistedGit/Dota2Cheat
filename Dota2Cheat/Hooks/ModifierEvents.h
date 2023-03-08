#pragma once
#include "../SDK/pch.h"
#include "../Modules/Hacks/EnemySpellHighlight.h"
#include "../Modules/Hacks/TargetedSpellHighlighter.h"
#include "../Modules/Hacks/LinearProjectileWarner.h"

namespace Hooks {
	inline Signatures::OnRemoveModifierFn oOnRemoveModifier = nullptr;
	inline Signatures::OnAddModifierFn oOnAddModifier = nullptr;
	inline bool HookedOnAddModifier = false;
		
	struct ImportantItemData {
		const char* itemName{};
		const char* modifierName{};
		CDOTAItem** importantItemPtr{};
		ImportantItemData(const char* itemName, const char* modifierName, CDOTAItem** importantItemPtr)
			:itemName(itemName), modifierName(modifierName), importantItemPtr(importantItemPtr) { }
	};
	void CacheIfItemModifier(CDOTAModifier* modifier);
	void hkOnAddModifier(CDOTAModifier* modifier, int unk);
	void hkOnRemoveModifier(CDOTAModifier* modifier, C_DOTA_PlayerResource* playerResource, void* unk);
}