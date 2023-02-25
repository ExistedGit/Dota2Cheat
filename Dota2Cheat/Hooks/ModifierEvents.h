#pragma once
#include "../Signatures.h"
#include "../Wrappers.h"
#include "../ValveClasses.h"
#include "../EnemySpellHighlight.h"

namespace Hooks {
	inline Signatures::OnRemoveModifierFn oOnRemoveModifier = nullptr;
	inline Signatures::OnAddModifierFn oOnAddModifier = nullptr;
	inline bool HookedOnAddModifier = false;

	void CacheIfItemModifier(DotaModifier* modifier) {
		std::string modName = modifier->GetName();
		if (modName.starts_with("modifier_item"))
		{
			auto itemName = modName.substr(9); // removing the "modifier_" prefix

			auto owner = modifier->GetOwner();
			auto foundItem = owner->FindItemBySubstring(itemName.c_str());
			if (foundItem.IsValid()) {
				if (strstr(foundItem.name, "midas")) {
					ctx.importantItems.midas = foundItem.GetAs<BaseAbility>();
				}
			}
		}
		else
			Modules::EnemySpellHighlighter.RenderIfThinkerModifier(modifier);
	}

	void hkOnAddModifier(DotaModifier* thisptr, int unk) {
		CacheIfItemModifier(thisptr);
		oOnAddModifier(thisptr, unk);
	}

	void hkOnRemoveModifier(DotaModifier* modifier, CDOTAPlayerResource* playerResource, void* unk) {
		if (!HookedOnAddModifier) {
			HookedOnAddModifier = true;
			void* onAddModifier = modifier->GetVFunc(35).ptr;

			HookFunc(onAddModifier, &hkOnAddModifier, &oOnAddModifier, "AddModifier");
		}

		std::string_view modName = modifier->GetName();
		if (modName.starts_with("modifier_item"))
		{
			auto itemName = modName.substr(9); // removing the "modifier_" prefix
			if (itemName.find("midas", 0) != std::string_view::npos &&
				modifier->GetOwner() == ctx.assignedHero) {
				ctx.importantItems.midas = nullptr;
			}

		}

		oOnRemoveModifier(modifier, playerResource, unk);
	}

}