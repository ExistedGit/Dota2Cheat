#include "AutoHeal.h"

void Modules::M_AutoHeal::UseWand(CDOTABaseNPC* hero) {
	if (!Config::AutoHeal::Enabled)
		return;

	if ((float)hero->GetHealth() / hero->GetMaxHealth() > Config::AutoHeal::WandHPTreshold / 100.0f)
		return;
	
	auto wand = hero->FindItemBySubstring("item_magic");
	if (!wand)
		wand = hero->FindItemBySubstring("holy_locket");

	if (!wand)
		return;

	if (wand->GetCooldown() != 0 || wand->GetCharges() < Config::AutoHeal::WandMinCharges)
		return;

	ctx.localPlayer->CastNoTarget(wand, hero);

}

void Modules::M_AutoHeal::UseFaerieFire(CDOTABaseNPC* hero) {
	if (!Config::AutoHeal::Enabled)
		return;

	if ((float)hero->GetHealth() / hero->GetMaxHealth() > Config::AutoHeal::FaerieFireHPTreshold / 100.0f)
		return;

	auto faerieFire = hero->FindItemBySubstring("faerie");
	if (!faerieFire)
		return;

	if (faerieFire->GetCooldown() != 0)
		return;

	ctx.localPlayer->CastNoTarget(faerieFire, hero);
}
