#include "KillIndicator.h"

void Modules::KillIndicator::DrawIndicatorFor(CDOTABaseNPC_Hero* hero) {
	if (hero->IsSameTeam(ctx.localHero)
		|| !hero->IsTargetable()
		|| hero->IsIllusion()
		|| !IsPointOnScreen(HeroData[hero].W2S))
		return;

	float scale = Config::Indicators::KillScale;
	const auto hbPos = HeroData[hero].HealthbarW2S;

	auto nuke = ctx.localHero->GetAbility(curData.idx);
	if (nuke->GetLevel() == 0)
		return;

	ID3D11ShaderResourceView* icon = assets.spellIcons.Load(nuke->GetIdentity()->GetName());

	int diff = 0;
	if (CustomBehaviors.contains(ctx.localHero->GetUnitName()))
		diff = CustomBehaviors.at(ctx.localHero->GetUnitName())(hero);
	else
		diff = DefaultBehavior(hero);

	auto indicatorBase = hbPos - ImVec2(76, 6);
	auto plaqueXY = indicatorBase - ImVec2(26, 0) * scale;
	auto DrawList = ImGui::GetForegroundDrawList();
	if (diff <= 0)
		// Outline
		DrawList->AddRectFilled(indicatorBase, indicatorBase + ImVec2(18, 18) * scale, ImColor(0, 230, 0, 255), (3 * scale));
	else {
		if (diff > 999)
			diff = 999;

		std::string Text = std::to_string(diff);
		auto textBase = plaqueXY + ImVec2(24 * scale / 2, (18 - 12) * scale / 2);

		// Outline
		DrawList->AddRectFilled(indicatorBase, indicatorBase + ImVec2(18, 18) * scale, ImColor(255, 50, 50, 255), (3 * scale));

		// Background & text
		DrawList->AddRectFilled(plaqueXY, plaqueXY + ImVec2(24, 18) * scale, ImColor(0, 0, 0, 255), 2 * scale);
		DrawTextForeground(DrawData.GetFont("Monofonto", scale * 16), Text, textBase, scale * 12, ImVec4(1, 1, 1, 1), true, false);
	}

	// Icon
	DrawList->AddRectFilled(indicatorBase + ImVec2{ 2,2 }, indicatorBase + ImVec2(16, 16) * scale, ImColor(1, 1, 1, 255), (2 * scale));
	DrawList->AddImageRounded(icon, indicatorBase + ImVec2{ 3,3 }, indicatorBase + ImVec2(15, 15) * scale, { 0, 0 }, { 1, 1 }, ImColor(255, 255, 255, 255), (2 * scale));
}
