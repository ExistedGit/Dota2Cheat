#pragma once
#include "../Signatures.h"
#include "../Wrappers.h"
#include "../ValveClasses.h"
#include "../EnemySpellHighlight.h"
#include "../TargetedSpellHighlighter.h"

namespace Hooks {
	inline Signatures::OnRemoveModifierFn oOnRemoveModifier = nullptr;
	inline Signatures::OnAddModifierFn oOnAddModifier = nullptr;
	inline bool HookedOnAddModifier = false;

	void CacheIfItemModifier(DotaModifier* modifier) {
		std::string_view modName = modifier->GetName();
		if (modName.starts_with("modifier_item"))
		{
			auto itemName = modName.substr(9); // removing the "modifier_" prefix
			auto owner = modifier->GetOwner();
			auto foundItem = owner->FindItemBySubstring(itemName.data());
			if (foundItem.IsValid()) {
				if (strstr(foundItem.name, "midas")) {
					ctx.importantItems.midas = foundItem.GetAs<BaseAbility>();
				}
				else if (strstr(foundItem.name, "sphere")) {
					Modules::TargetedSpellHighlighter.SubscribeLinkenRendering(owner, foundItem.GetAs<BaseAbility>());
				}
			}
		}
	}

	void hkOnAddModifier(DotaModifier* modifier, int unk) {
		CacheIfItemModifier(modifier);
		Modules::TargetedSpellHighlighter.DrawParticleIfTargetedSpell(modifier);
		Modules::EnemySpellHighlighter.RenderIfThinkerModifier(modifier);
		Modules::LinearProjectileWarner.DrawIfTrajectoryModifier(modifier);
		oOnAddModifier(modifier, unk);
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
			if (itemName.find("midas", 0) != -1 &&
				modifier->GetOwner() == ctx.assignedHero) {
				ctx.importantItems.midas = nullptr;
			}
			else if (itemName.find("sphere", 0) != -1) {
				Modules::TargetedSpellHighlighter.UnsubscribeLinkenRendering(modifier->GetOwner());
			}

		}

		Modules::TargetedSpellHighlighter.RemoveParticleIfTargetedSpell(modifier);
		Modules::LinearProjectileWarner.RemoveParticleIfTrajectoryModifier(modifier);

		oOnRemoveModifier(modifier, playerResource, unk);
	}

}