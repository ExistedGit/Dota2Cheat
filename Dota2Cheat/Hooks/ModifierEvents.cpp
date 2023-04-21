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
	if (modifier->GetOwner() == ctx.assignedHero)
	{
		auto searchName = importantItemNames.count(modName) ?
			importantItemNames[modName] :
			itemName.substr(5);
		auto item = modifier->GetOwner()->FindItemBySubstring(searchName.c_str());
		if (item)
			ctx.ImportantItems[searchName] = item;
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
	Modules::TrueSightESP.DrawParticleIfTrueSight(modifier);
	Modules::EnemySpellHighlighter.RenderIfThinkerModifier(modifier);
	Modules::LinearProjectileWarner.DrawIfTrajectoryModifier(modifier);

	oOnAddModifier(modifier, unk);
}

void Hooks::hkOnRemoveModifier(CDOTAModifier* modifier) {
	//if (!oOnAddModifier) {
	//	void* OnAddModifier = modifier->GetVFunc(VTableIndexes::CDOTA_Buff::OnAddModifier).ptr;
	//	HOOKFUNC(OnAddModifier);
	//}

	std::string modName = modifier->GetName();

	if (modName.starts_with("modifier_item"))
	{
		auto itemName = modName.substr(9); // removing the "modifier_" prefix
		if (modifier->GetOwner() == ctx.assignedHero) {
			auto searchName = importantItemNames.count(modName) ?
				importantItemNames[modName] :
				itemName.substr(5);
			ctx.ImportantItems.erase(searchName);
		}

		if (itemName.find("sphere", 0) != -1)
			Modules::TargetedSpellHighlighter.UnsubscribeLinkenRendering(modifier->GetOwner());
	}

	Modules::TargetedSpellHighlighter.RemoveParticleIfTargetedSpell(modifier);
	Modules::LinearProjectileWarner.RemoveParticleIfTrajectoryModifier(modifier);
	Modules::TrueSightESP.RemoveParticleIfTrueSight(modifier);

	oOnRemoveModifier(modifier);
}
