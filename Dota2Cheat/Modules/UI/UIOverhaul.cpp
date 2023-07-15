#include "UIOverhaul.h"

CDOTABaseNPC_Hero* Modules::UIOverhaul::FindHeroByUnitName(std::string_view name) {
	//for (auto& hero : ctx.heroes) {
	//	auto unitName = hero->GetUnitName();
	//	if (unitName && unitName == name)
	//		return hero;
	//}
	return nullptr;
}

CUIPanel* Modules::UIOverhaul::GetTopBarImgForHero(CDOTABaseNPC_Hero* hero) {
	auto topbarImages = Panorama::DotaHud->FindChildrenWithClassTraverse(TopBarClass);
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
	topBar.clear();
	auto topbarImages = Panorama::DotaHud->FindChildrenWithClassTraverse(TopBarClass);
	for (auto& panel : topbarImages) {
		if (!panel->GetId() || strcmp(panel->GetId(), "HeroImage") != 0)
			continue;

		auto heroImg = (CDOTA_UI_HeroImage*)panel->GetPanel2D();
		//LogF(LP_DATA, "TopBarHeroImage: {} {}", (void*)panel, (void*)panel->GetPanel2D());
		if (!IsValidReadPtr(heroImg->GetSrc()))
			continue;
		std::string heroName(heroImg->GetSrc());
		heroName = heroName.substr(23, heroName.size() - 23 - 4);
		//LogF(LP_DATA, "\tHERO: {} XY: {} {}", heroName, POS.x, POS.y);
		auto hero = FindHeroByUnitName(heroName);
		if (hero)
			topBar[hero] = panel;
	}
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

	if (!TopBarClass)
		TopBarClass = Interfaces::UIEngine->MakeSymbol("TopBarHeroImage");

	UpdateHeroes();
}
