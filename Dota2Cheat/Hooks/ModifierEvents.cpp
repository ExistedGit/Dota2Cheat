#include "ModifierEvents.h"


void Hooks::CacheIfItemModifier(CDOTAModifier* modifier) {
	static const std::vector<ImportantItemData> importantItemNames = {
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
			},
			{
				"item_armlet",
				"modifier_item_armlet",
				&ctx.importantItems.armlet
			},
			{
				"item_power_treads",
				"modifier_item_power_treads",
				&ctx.importantItems.power_treads
			},
			{
				"item_vambrace",
				"modifier_item_vambrace",
				&ctx.importantItems.vambrace
			}
	};
	std::string_view modName = modifier->GetName();
	if (!modName.starts_with("modifier_item"))
		return;

	if (modifier->GetOwner() == ctx.assignedHero)
		for (auto& data : importantItemNames) {
			if (data.modifierName != modName)
				continue;
			auto item = modifier->GetOwner()->FindItemBySubstring(data.itemName);
			if (!item)
				break;

			*data.importantItemPtr = item;
		}
	auto itemName = modName.substr(9); // removing the "modifier_" prefix
	auto item = modifier->GetOwner()->FindItemBySubstring(itemName.data());
	if (item) {
		if (itemName.find("sphere", 0) != -1)
			Modules::TargetedSpellHighlighter.SubscribeLinkenRendering(modifier->GetOwner(), item);
	}

}

void Hooks::hkOnAddModifier(CDOTAModifier* modifier, int unk) {
	CacheIfItemModifier(modifier);
	Modules::TargetedSpellHighlighter.DrawParticleIfTargetedSpell(modifier);
	Modules::EnemySpellHighlighter.RenderIfThinkerModifier(modifier);
	Modules::LinearProjectileWarner.DrawIfTrajectoryModifier(modifier);
	oOnAddModifier(modifier, unk);
}

void Hooks::hkOnRemoveModifier(CDOTAModifier* modifier, C_DOTA_PlayerResource* playerResource, void* unk) {
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
