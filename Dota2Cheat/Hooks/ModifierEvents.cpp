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

	auto item = modifier->GetOwner()->FindItemBySubstring(itemName.data());
	if (item) {
		if (itemName.find("sphere", 0) != -1 &&
			!((CDOTABaseNPC_Hero*)modifier->GetOwner())->IsIllusion())
			Modules::TargetedSpellHighlighter.SubscribeLinkenRendering(modifier->GetOwner(), item);
	}

}

void Hooks::hkOnAddModifier(CDOTAModifier* modifier, int unk) {
	if (ctx.gameStage == GameStage::IN_GAME) {
		CacheIfItemModifier(modifier);
		CacheModifier(modifier);
		Modules::TargetedSpellHighlighter.DrawParticleIfTargetedSpell(modifier);
		Modules::TrueSightESP.DrawParticleIfTrueSight(modifier);
		Modules::EnemySpellHighlighter.RenderIfThinkerModifier(modifier);
		Modules::LinearProjectileWarner.DrawIfTrajectoryModifier(modifier);
	}

	oOnAddModifier(modifier, unk);
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

			if (itemName.find("sphere", 0) != -1)
				Modules::TargetedSpellHighlighter.UnsubscribeLinkenRendering(modifier->GetOwner());
		}
		UncacheModifier(modifier);
		Modules::TargetedSpellHighlighter.RemoveParticleIfTargetedSpell(modifier);
		Modules::LinearProjectileWarner.RemoveParticleIfTrajectoryModifier(modifier);
		Modules::TrueSightESP.RemoveParticleIfTrueSight(modifier);
	}
	oOnRemoveModifier(modifier);
}
