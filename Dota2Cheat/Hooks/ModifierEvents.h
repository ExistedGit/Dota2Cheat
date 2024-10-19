#pragma once
#include "../CheatSDK/include.h"

#include "../Modules/Hacks/EnemySpellHighlight.h"
#include "../Modules/Hacks/TargetedSpellHighlighter.h"
#include "../Modules/Hacks/LinearProjectileWarner.h"
#include "../Modules/Hacks/TrueSightESP.h"

namespace Hooks {
	inline void* oOnRemoveModifier = nullptr;
	inline void* oOnAddModifier = nullptr;

	void CacheModifier(CDOTAModifier* modifier);
	void UncacheModifier(CDOTAModifier* modifier);

	void CacheIfItemModifier(CDOTAModifier* modifier);
	void hkOnAddModifier(CDOTAModifier* modifier, int unk);
	void hkOnRemoveModifier(CDOTAModifier* modifier);
}