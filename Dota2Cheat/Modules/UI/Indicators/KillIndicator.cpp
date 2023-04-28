#include "KillIndicator.h"

void Hacks::KillIndicator::DrawIndicatorFor(CDOTABaseNPC* ent) {
	if (!ent)
		return;

	float scale = 1.2f;
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
	auto DrawList = ImGui::GetForegroundDrawList();
	if (diff <= 0)
		// Outline
		DrawList->AddRectFilled(indicatorBase - ImVec2{ 1,1 }, indicatorBase + ImVec2(18, 18) * scale, ImColor(0, 230, 0, 255), (3 * scale));
	else {
		if (diff > 999)
			diff = 999;
		std::string Text = std::to_string(diff);
		auto textBase = indicatorBase + ImVec2{ 18 * scale + 4 , 1 };
		//Outline
		DrawList->AddRectFilled(indicatorBase - ImVec2{ 1,1 }, indicatorBase + ImVec2(18, 18) * scale, ImColor(255, 50, 50, 255), (3 * scale));
		// Background & text
		DrawList->AddRectFilled(textBase, textBase + ImVec2{scale * 8 * Text.size(), scale * 16}, ImColor(0, 0, 0, 255), 2 * scale);
		DrawList->AddText(GImGui->Font, scale*16, textBase + ImVec2{2,0}, ImColor(255, 255, 255, 255), Text.c_str());
	}
	//Icon
	DrawList->AddRectFilled(indicatorBase + ImVec2{ 1,1 }, indicatorBase + ImVec2(16, 16) * scale, ImColor(1, 1, 1, 255), (2 * scale));
	DrawList->AddImageRounded(icon, indicatorBase + ImVec2{ 2,2 }, indicatorBase + ImVec2(15, 15) * scale, { 0, 0 }, { 1, 1 }, ImColor(255, 255, 255, 255), (2 * scale));
}
