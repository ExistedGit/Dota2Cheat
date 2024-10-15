#include "UIOverhaul.h"

void Modules::UIOverhaul::UpdateNetworthPanel() {
	if (!NWPanelStateQueued)
		return;

	if (auto panel = Panorama::DotaHud->FindChildByIdTraverse("SpectatorGoldDisplay"))
		panel->SetActive(Config::UIOverhaul::NetworthPanel);

	NWPanelStateQueued = false;
}

// Mana and Health bars
void Modules::UIOverhaul::Draw() {
	if (!Config::UIOverhaul::TopBars)
		return;

	MTM_LOCK;
	auto dl = ImGui::GetBackgroundDrawList();
	constexpr static int barHeight = 8; // as in the game
	for (const auto& data : renderQueue) {
		ImVec2 imgXY1 = data.pos;
		if (!data.isDire)
			imgXY1.x += topBarImgSlope;

		float manaRatio = data.mana / data.manaMax,
			hpRatio = data.health / data.healthMax;

		ImVec2 healthBarXY{ imgXY1.x, imgXY1.y + topBarImgSize.y },
			manaBarXY{ imgXY1.x, imgXY1.y + topBarImgSize.y + barHeight };

		DrawRectFilled(healthBarXY, { topBarImgSize.x, barHeight }, ImVec4(0, 0, 0, 1));
		DrawRectFilled({ healthBarXY.x + 1, healthBarXY.y + 1 }, { (topBarImgSize.x - 2) * hpRatio  , barHeight - 2 }, ImVec4(123 / 255.f, 222 / 255.f, 50 / 255.f, 1));
		DrawRectFilled(manaBarXY, { topBarImgSize.x, barHeight }, ImVec4(0, 0, 0, 1));
		DrawRectFilled({ manaBarXY.x + 1, manaBarXY.y + 1 }, { (topBarImgSize.x - 2) * manaRatio , barHeight - 2 }, ImVec4(0, 0.5, 1, 1));
	}

	renderQueue.clear();
}

void Modules::UIOverhaul::Init() {
	if (!Panorama::DotaHud)
		return;

	MTM_LOCK;


}

void Modules::UIOverhaul::OnFrame() {
	return;

	if (!Config::UIOverhaul::TopBars)
		return;

	if (!ctx.localHero)
		return;

	MTM_LOCK;
	//UpdateNetworthPanel();

	panels.clear();
	panels.reserve(10);

	auto topbar = Panorama::DotaHud->FindChildByIdTraverse("topbar");

	uint16_t cl = CUIEngine::Get()->MakeSymbol("TopBarPlayerSlot");
	topbar->FindChildById("TopBarDireTeamContainer")->FindChildrenWithClassTraverse(cl, panels);
	topbar->FindChildById("TopBarRadiantTeamContainer")->FindChildrenWithClassTraverse(cl, panels);

	for (const auto panel : panels) {
		const auto tbPlayer = (CDOTA_UI_Top_Bar_Player*)panel->GetPanel2D();
		if (!tbPlayer->GetHero().IsValid()) continue;

		CHero* hero = tbPlayer->GetHero();
		if (hero->IsSameTeam(ctx.localHero) || hero->GetLifeState() != 0) continue;

		RenderData data{
			.pos = panel->GetPanel2D()->GetPositionWithinWindow(),
			.isDire = true,
			.isDormant = hero->GetIdentity()->IsDormant(),
			.health = (float)hero->GetHealth(),
			.mana = (float)hero->GetMana(),
			.healthMax = (float)hero->GetMaxHealth(),
			.manaMax = (float)hero->GetMaxMana()
		};

		renderQueue.push_back(data);
	}
}
