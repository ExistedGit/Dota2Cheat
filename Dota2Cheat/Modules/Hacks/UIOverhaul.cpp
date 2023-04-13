#include "UIOverhaul.h"

CDOTABaseNPC_Hero* Hacks::UIOverhaul::FindHeroByUnitName(std::string_view name) {
	for (auto& hero : ctx.heroes) {
		auto unitName = hero->GetUnitName();
		if (unitName && unitName == name)
			return hero;
	}
	return nullptr;
}

CUIPanel* Hacks::UIOverhaul::GetTopBarImgForHero(CDOTABaseNPC_Hero* hero) {
	auto topbarImages = DotaHud->FindChildrenWithClassTraverse(PClass::TopBarHeroImage);
	for (auto& panel : topbarImages) {
		if (!panel->GetId() || strcmp(panel->GetId(), "HeroImage") != 0)
			continue;

		auto heroImg = (Panorama::CDOTA_UI_HeroImage*)panel->GetPanel2D();
		if (!IsValidReadPtr(heroImg->GetSrc()))
			continue;

		std::string heroName(heroImg->GetSrc());
		heroName = heroName.substr(23, heroName.size() - 23 - 4);
		if (heroName == hero->GetUnitName())
			return panel;
	}
	return nullptr;
}

// Mana and Health bars
void Hacks::UIOverhaul::DrawBars() {
	if (!Intiialized)
		return; 
	auto DrawList = ImGui::GetForegroundDrawList();
	constexpr static int barHeight = 8; // as in the game
	for (auto& [hero, data] : topBar) {
		if (hero->IsSameTeam(ctx.assignedHero))
			continue;

		auto heroImg = data.panel->GetPanel2D();
		ImVec2 imgXY1 = ImVecFromVec2D(heroImg->GetPositionWithinWindow());
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

void Hacks::UIOverhaul::Init() {
	for (auto& node : Interfaces::UIEngine->GetPanelList<4096>()) {
		auto uiPanel = node.uiPanel;
		if (!uiPanel->GetId())
			continue;
		std::string_view id = uiPanel->GetId();
		if (id != "Hud")
			continue;

		DotaHud = uiPanel;
		LogF(LP_DATA, "Hud: {}", (void*)DotaHud->GetPanel2D());
	}

	UpdateHeroes();
	Intiialized = true;
}
