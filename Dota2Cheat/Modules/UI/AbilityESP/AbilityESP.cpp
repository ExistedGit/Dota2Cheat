#include "AbilityESP.h"
#include <format>

bool Modules::M_AbilityESP::CanDraw(CDOTABaseNPC_Hero* hero) {
	bool ret =
		hero
		&& !hero->GetIdentity()->IsDormant()
		&& !hero->IsIllusion()
		&& hero != ctx.localHero
		&& hero->GetLifeState() == 0
		&& IsPointOnScreen(HeroData[hero].W2S);
	if (!Config::AbilityESP::ShowAllies)
		// I wish they made &&= an operator
		ret = ret && !hero->IsSameTeam(ctx.localHero);
	return ret;
}

void Modules::M_AbilityESP::DrawChargeCounter(int charges, const ImVec2& pos, int radius) {
	auto DrawList = ImGui::GetForegroundDrawList();

	// Green outline
	// DrawList->AddCircleFilled(pos, radius + 1, ImGui::GetColorU32(ImVec4(135 / 255.0f, 214 / 255.0f, 77 / 255.0f, 1)));
	// Gray core
	// DrawList->AddCircleFilled(pos, radius, ImGui::GetColorU32(ImVec4(0.2, 0.2, 0.2, 1)));
	auto textSize = ScaleVar(20);
	DrawTextForeground(
		DrawData.GetFont("Monofonto", textSize),
		std::to_string(charges),
		ImVec2(pos.x, pos.y - textSize / 2),
		textSize,
		ImVec4(1, 1, 1, 1),
		true);
}

void Modules::M_AbilityESP::DrawHeroItems(CHero* hero) {
	auto itemType = (ItemPanelType)Config::AbilityESP::ItemPanelType;

	if (itemType == ItemPanelType::Sequence)
		DrawItemSequences(hero);
	else
		DrawItemGrids(hero);
}

void Modules::M_AbilityESP::DrawESP() {
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