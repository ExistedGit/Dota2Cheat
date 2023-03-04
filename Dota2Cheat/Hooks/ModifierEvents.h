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


	struct ImportantItemInfo {
		const char* itemName{};
		const char* modifierName{};
		BaseAbility** importantItemPtr{};
		ImportantItemInfo(const char* itemName, const char* modifierName, BaseAbility** importantItemPtr)
			:itemName(itemName), modifierName(modifierName), importantItemPtr(importantItemPtr) {

		}
	};
	void CacheIfItemModifier(DotaModifier* modifier) {
		static const std::vector<ImportantItemInfo> importantItemNames = {
			{
				"item_hand_of_midas",
				"modifier_item_hand_of_midas",
				&ctx.importantItems.midas
			},
			{
				"item_manta",
				"modifier_item_manta_style",
				&ctx.importantItems.manta
			},
			{
				"item_bottle",
				"modifier_item_empty_bottle",
				&ctx.importantItems.bottle
			}
		};
		std::string_view modName = modifier->GetName();
		if (modifier->GetOwner() != ctx.assignedHero ||
			!modName.starts_with("modifier_item"))
			return;
	
		for (auto& info : importantItemNames) {
			if (info.modifierName != modName)
				continue;
			auto item = modifier->GetOwner()->FindItemBySubstring(info.itemName);
			if (!item.IsValid())
				break;

			*info.importantItemPtr = item.GetAs<BaseAbility>();
		}

	}

	void hkOnAddModifier(DotaModifier* modifier, int unk) {
		CacheIfItemModifier(modifier);
		Modules::TargetedSpellHighlighter.DrawParticleIfTargetedSpell(modifier);
		Modules::EnemySpellHighlighter.RenderIfThinkerModifier(modifier);
		Modules::LinearProjectileWarner.DrawIfTrajectoryModifier(modifier);
		oOnAddModifier(modifier, unk);
	}

	void hkOnRemoveModifier(DotaModifier* modifier, C_DOTA_PlayerResource* playerResource, void* unk) {
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