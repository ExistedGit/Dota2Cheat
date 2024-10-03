#include "AbilityESP.h"
#include "../../../Utils/Drawing.h"
#include <format>

bool Modules::M_AbilityESP::CanDraw(CDOTABaseNPC_Hero* hero) {
	bool ret =
		hero
		&& !hero->GetIdentity()->IsDormant()
		&& !hero->IsIllusion()
#ifndef _DEBUG
		&& hero != ctx.localHero
#endif
		&& hero->GetLifeState() == 0
		&& IsPointOnScreen(HeroData[hero].W2S);
	if (!Config::AbilityESP::ShowAllies)
		// I wish they made &&= an operator
		ret = ret && !hero->IsSameTeam(ctx.localHero);
	return ret;
}

void Modules::M_AbilityESP::DrawHeroItems(CHero* hero) {
	auto itemType = (ItemPanelType)Config::AbilityESP::ItemPanelType;

	if (itemType == ItemPanelType::Sequence)
		DrawItemSequences(hero);
	else
		DrawItemGrids(hero);
}

void Modules::M_AbilityESP::DrawChargeCounterBasic(int charges, const ImVec2& pos) {
	auto textSize = ScaleVar(20);
	DrawTextForeground(
		DrawData.GetFont("Monofonto", textSize),
		std::to_string(charges),
		ImVec2(pos.x, pos.y - textSize / 2),
		textSize,
		ImVec4(1, 1, 1, 1),
		true);
}

void Modules::M_AbilityESP::DrawChargeCounterImmersive(int charges, const ImVec2& pos) {
	auto DrawList = ImGui::GetForegroundDrawList();

	int counterScale = ScaleVar(16) / 2;
	int counterTextScale = ScaleVar(14);
	// Green outline
	DrawList->AddCircleFilled(pos, counterScale + 1, ImColor(135, 214, 77));
	// Gray core
	DrawList->AddCircleFilled(pos, counterScale, ImColor(0.2f, 0.2f, 0.2f, 1.0f));
	auto textSize = ScaleVar(20);
	DrawTextForeground(
		DrawData.GetFont("Monofonto", counterTextScale),
		std::to_string(charges),
		ImVec2(pos.x, pos.y - counterTextScale / 2 - counterTextScale % 2),
		counterTextScale,
		ImVec4(1, 1, 1, 1),
		true);
}


void Modules::M_AbilityESP::Draw() {
	if (!Config::AbilityESP::Enabled)
		return;

	EntityList.ForEach<CHero>(
		[this](CHero* hero) {
			if (!CanDraw(hero)) return;

			DrawHeroAbilities(hero);
			DrawHeroItems(hero);
		}
	);
}