#include "ModifierEvents.h"

//
inline std::map<std::string, std::string> importantItemNames = {
		{
			"modifier_item_manta_style",
			"manta"
		},
		{
			"modifier_item_empty_bottle",
			"bottle"
		}
};

void Hooks::CacheIfItemModifier(CDOTAModifier* modifier) {
	std::string modName = modifier->GetName();
	if (!modName.starts_with("modifier_item"))
		return;

	auto itemName = modName.substr(9); // removing the "modifier_" prefix
	if (modifier->GetOwner() == ctx.localHero)
	{
		auto searchName = importantItemNames.count(modName) ?
			importantItemNames[modName] :
			itemName.substr(5);
		auto item = modifier->GetOwner()->FindItemBySubstring(searchName.c_str());
		if (item)
			HeroData[ctx.localHero].Items[searchName] = item;
	}

	if (modName == "modifier_item_sphere") {
		auto item = modifier->GetOwner()->FindItem("item_sphere");
		if (item && !((CDOTABaseNPC_Hero*)modifier->GetOwner())->IsIllusion())
			Modules::TargetedSpellHighlighter.SubscribeLinkenRendering(modifier->GetOwner(), item);
	}

}

void Hooks::hkOnAddModifier(CDOTAModifier* modifier, int unk) {
	if (ctx.gameStage == GameStage::IN_GAME) {
		CacheIfItemModifier(modifier);
		CacheModifier(modifier);
		Modules::TargetedSpellHighlighter.OnModifierAdded(modifier);
		Modules::TrueSightESP.OnModifierAdded(modifier);
		Modules::EnemySpellHighlighter.OnModifierAdded(modifier);
		Modules::LinearProjectileWarner.OnModifierAdded(modifier);
	}

	((decltype(&hkOnAddModifier))oOnAddModifier)(modifier, unk);
}

void Hooks::hkOnRemoveModifier(CDOTAModifier* modifier) {

	if (ctx.gameStage == GameStage::IN_GAME) {
		std::string modName = modifier->GetName();

		if (modName.starts_with("modifier_item"))
		{
			auto itemName = modName.substr(9); // removing the "modifier_" prefix
			if (modifier->GetOwner() == ctx.localHero) {
				auto searchName = importantItemNames.count(modName) ?
					importantItemNames[modName] :
					itemName.substr(5);
				HeroData[ctx.localHero].Items.erase(searchName);
			}

			if (itemName == "item_sphere")
				Modules::TargetedSpellHighlighter.UnsubscribeLinkenRendering(modifier->GetOwner());
		}
		UncacheModifier(modifier);
		Modules::TargetedSpellHighlighter.OnModifierRemoved(modifier);
		Modules::LinearProjectileWarner.OnModifierRemoved(modifier);
		Modules::TrueSightESP.OnModifierRemoved(modifier);
	}
	((decltype(&hkOnRemoveModifier))oOnRemoveModifier)(modifier);
}
