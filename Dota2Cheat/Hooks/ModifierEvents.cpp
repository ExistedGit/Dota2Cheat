#include "ModifierEvents.h"

inline std::map<std::string_view, Hooks::ImportantItemData> importantItemNames = {
		{
			"modifier_item_hand_of_midas",
			{
			"item_hand_of_midas",
			&ctx.importantItems.midas
			}
		},
		{
			"modifier_item_manta_style",
			{
			"item_manta",
			&ctx.importantItems.manta
			}
		},
		{
			"modifier_item_empty_bottle",
			{
			"item_bottle",
			&ctx.importantItems.bottle
			}
		},
		{
			"item_armlet",
			{
			"modifier_item_armlet",
			&ctx.importantItems.armlet
			}
		},
		{
			"modifier_item_power_treads",
			{

			"item_power_treads",
			&ctx.importantItems.power_treads
			}
		},
		{
			"modifier_item_vambrace",
			{
			"item_vambrace",
			&ctx.importantItems.vambrace
			}
		}
};

void Hooks::CacheIfItemModifier(CDOTAModifier* modifier) {
	std::string_view modName = modifier->GetName();
	auto itemName = modName.substr(9); // removing the "modifier_" prefix

	if (!modName.starts_with("modifier_item"))
		return;

	if (modifier->GetOwner() == ctx.assignedHero
		&& importantItemNames.count(modName))
	{
		auto& data = importantItemNames[modName];
		auto item = modifier->GetOwner()->FindItemBySubstring(data.itemName);
		if (!item)
			return;
		*data.item = item;
	}

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
	if (!oOnAddModifier) {
		void* onAddModifier = modifier->GetVFunc(VTableIndexes::CDOTA_Buff::OnAddModifier).ptr;
		HookFunc(onAddModifier, &hkOnAddModifier, &oOnAddModifier, "OnAddModifier");
	}

	std::string_view modName = modifier->GetName();
	auto itemName = modName.substr(9); // removing the "modifier_" prefix

	if (modName.starts_with("modifier_item") && modifier->GetOwner() == ctx.assignedHero)
	{

		if (modifier->GetOwner() == ctx.assignedHero && importantItemNames.count(modName))
			*importantItemNames[modName].item = nullptr;
		else if (itemName.find("sphere", 0) != -1)
			Modules::TargetedSpellHighlighter.UnsubscribeLinkenRendering(modifier->GetOwner());
	}

	Modules::TargetedSpellHighlighter.RemoveParticleIfTargetedSpell(modifier);
	Modules::LinearProjectileWarner.RemoveParticleIfTrajectoryModifier(modifier);


	oOnRemoveModifier(modifier, playerResource, unk);
}
