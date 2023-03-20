#pragma once
#include "../SDK/pch.h"
#include "../Modules/Hacks/EnemySpellHighlight.h"
#include "../Modules/Hacks/TargetedSpellHighlighter.h"
#include "../Modules/Hacks/LinearProjectileWarner.h"

namespace Hooks {
	inline Signatures::OnRemoveModifierFn oOnRemoveModifier = nullptr;
	inline Signatures::OnAddModifierFn oOnAddModifier = nullptr;
		
	struct ImportantItemData {
		const char* itemName{};
		CDOTAItem** item{};
		ImportantItemData() {};
		ImportantItemData(const char* itemName, CDOTAItem** item)
			:itemName(itemName), item(item) { }
	};
	void CacheIfItemModifier(CDOTAModifier* modifier);
	void hkOnAddModifier(CDOTAModifier* modifier, int unk);
	void hkOnRemoveModifier(CDOTAModifier* modifier);
}