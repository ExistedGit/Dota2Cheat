#include "UIOverhaul.h"

CUIPanel* Modules::UIOverhaul::GetTopBarImgForHero(CDOTABaseNPC_Hero* hero) {
	auto topbarImages = Panorama::DotaHud->FindChildrenWithIdTraverse("HeroImage");
	for (auto& panel : topbarImages) {
		if (!panel->GetId() || strcmp(panel->GetId(), "HeroImage") != 0)
			continue;

		auto heroImg = (CDOTA_UI_HeroImage*)panel->GetPanel2D();
		if (!IsValidReadPtr(heroImg->GetSrc()))
			continue;

		std::string heroName(heroImg->GetSrc());
		heroName = heroName.substr(23, heroName.size() - 23 - 4);
		if (heroName == hero->GetUnitName())
			return panel;
	}
	return nullptr;
}

void Modules::UIOverhaul::UpdateHeroes() {
	static auto classSym = Interfaces::UIEngine->MakeSymbol("TopBarHeroImage");
	topBar.clear();
	auto topbarImages = Panorama::DotaHud->FindChildrenWithClassTraverse(classSym);
	for (auto& panel : topbarImages) {
		if (!panel->GetId() || strcmp(panel->GetId(), "HeroImage") != 0)
			continue;

		auto heroImg = (CDOTA_UI_HeroImage*)panel->GetPanel2D();

		auto hero = EntityList.Find<CDOTABaseNPC_Hero>([this, heroImg](auto* hero) {
			return hero->Member<int>(Netvars::C_DOTA_BaseNPC_Hero::m_iHeroID) == heroImg->GetHeroID();
			});

		if (hero)
			topBar[hero] = panel;
	}
}

void Modules::UIOverhaul::UpdateNetworthPanel() {
	if (!NWPanelStateQueued)
		return;

	if (auto panel = Panorama::DotaHud->FindChildWithIdTraverse("SpectatorGoldDisplay"))
		panel->SetActive(Config::UIOverhaul::NetworthPanel);

	NWPanelStateQueued = false;
}

// Mana and Health bars
void Modules::UIOverhaul::DrawBars() {
	if (!Config::UIOverhaul::TopBars)
		return;

	MTM_LOCK;
	auto DrawList = ImGui::GetForegroundDrawList();
	constexpr static int barHeight = 8; // as in the game
	for (auto& [hero, data] : topBar) {
		if (!IsValidReadPtr(hero) ||
			!IsValidReadPtr(hero->GetIdentity()) ||
			hero->IsSameTeam(ctx.localHero) ||
			!hero->IsTargetable())
			continue;
		if (!Interfaces::UIEngine->IsValidPanelPointer(data.panel))
			continue;

		ImVec2 imgXY1 = data.imgPos;
		if (!data.IsDire)
			imgXY1.x += topBarImgSlope;

		float manaRatio = hero->GetMana() / hero->GetMaxMana(),
			hpRatio = (float)hero->GetHealth() / hero->GetMaxHealth();

		ImVec2 healthBarXY{ imgXY1.x, imgXY1.y + topBarImgSize.y },
			manaBarXY{ imgXY1.x, imgXY1.y + topBarImgSize.y + barHeight };

		DrawRectFilled(healthBarXY, { topBarImgSize.x, barHeight }, ImVec4(0, 0, 0, 1));
		DrawRectFilled({ healthBarXY.x + 1, healthBarXY.y + 1 }, { (topBarImgSize.x - 2) * hpRatio  , barHeight - 2 }, ImVec4(123 / 255.f, 222 / 255.f, 50 / 255.f, 1));
		DrawRectFilled(manaBarXY, { topBarImgSize.x, barHeight }, ImVec4(0, 0, 0, 1));
		DrawRectFilled({ manaBarXY.x + 1, manaBarXY.y + 1 }, { (topBarImgSize.x - 2) * manaRatio , barHeight - 2 }, ImVec4(0, 0.5, 1, 1));
	}
}

void Modules::UIOverhaul::Init() {
	if (!Panorama::DotaHud)
		return;

	MTM_LOCK;

	UpdateHeroes();
}
