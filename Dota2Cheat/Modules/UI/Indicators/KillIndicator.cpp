#include "KillIndicator.h"

void Hacks::KillIndicator::DrawIndicatorFor(CDOTABaseNPC* ent) {
	if (!ent)
		return;

	float scale = Config::Indicators::KillScale;
	Vector pos = ent->GetPos();
	pos.z += ent->Member<int>(Netvars::C_DOTA_BaseNPC::m_iHealthBarOffset);
	const auto hbPos = WorldToScreen(pos);
	auto nuke = ctx.assignedHero->GetAbility(curData.idx);
	if (nuke->GetLevel() == 0)
		return;
	ImTextureID icon = 0;
	{
		auto abilityName = nuke->GetIdentity()->GetName();
		if (abilityName) {
			if (icon = texManager.GetNamedTexture(abilityName); !icon) {
				auto iconPath = ctx.cheatFolderPath + "\\assets\\spellicons\\" + abilityName + "_png.png";
				texManager.LoadTextureNamed(iconPath.c_str(), icon, abilityName);
			}
		}
	}

	int diff = 0;
	if (CustomBehaviors.count(ctx.assignedHero->GetUnitName()))
		diff = CustomBehaviors.at(ctx.assignedHero->GetUnitName())(ent);
	else
		diff = DefaultBehavior(ent);

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
		//Outline
		DrawList->AddRectFilled(indicatorBase, indicatorBase + ImVec2(18, 18) * scale, ImColor(255, 50, 50, 255), (3 * scale));
		// Background & text
		DrawList->AddRectFilled(plaqueXY, plaqueXY + ImVec2(24, 18) * scale, ImColor(0, 0, 0, 255), 2 * scale);
		DrawTextForeground(DrawData.GetFont("Monofonto", scale * 16), Text, textBase, scale * 12, ImVec4(1, 1, 1, 1), true, false);
	}
	//Icon
	DrawList->AddRectFilled(indicatorBase + ImVec2{ 2,2 }, indicatorBase + ImVec2(16, 16) * scale, ImColor(1, 1, 1, 255), (2 * scale));
	DrawList->AddImageRounded(icon, indicatorBase + ImVec2{ 3,3 }, indicatorBase + ImVec2(15, 15) * scale, { 0, 0 }, { 1, 1 }, ImColor(255, 255, 255, 255), (2 * scale));
}
