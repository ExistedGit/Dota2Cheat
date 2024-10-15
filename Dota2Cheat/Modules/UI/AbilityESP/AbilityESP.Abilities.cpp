#include "AbilityESP.h"
#include "../../../Utils/Drawing.h"

void Modules::M_AbilityESP::DrawLevelCounterImmersive(const RenderData::Ability& ability, const ImVec2& pos)  const {
	int lvl = ability.level;

	const auto clrLvlOutline = ImColor(231, 210, 146);
	const auto clrLvlBackground = ImColor(40, 15, 1);
	const ImVec2 outlinePadding(1, 1);
	int counterScale = ScaleVar(16);
	int counterTextScale = ScaleVar(14);

	ImVec2 counterSize(counterScale, counterScale);
	ImVec2 imgXY1 = pos - counterSize / 2, imgXY2 = pos + counterSize / 2;

	DrawRectFilled(imgXY1, counterSize, clrLvlBackground);
	DrawRect(
		imgXY1 + outlinePadding,
		counterSize - outlinePadding * 2,
		clrLvlOutline
	);

	ImDrawText(DrawData.GetFont("Monofonto", counterTextScale), std::to_string(lvl),
		ImVec2{ pos.x, pos.y - counterTextScale / 2 - counterTextScale % 2 },
		counterTextScale,
		clrLvlOutline,
		true);
}

void Modules::M_AbilityESP::DrawLevelCounterBasic(const RenderData::Ability& ability, const ImVec2& pos) const {
	int counterScale = ScaleVar(26);
	ImDrawText(DrawData.GetFont("Monofonto", counterScale - 2), std::to_string(ability.level),
		ImVec2(pos.x, pos.y - (ScaleVar(32) - 2) / 2),
		counterScale - 2,
		ImColor{ 255,255,255 },
		true);
}

void Modules::M_AbilityESP::DrawLevelBars(const RenderData::Ability& ability, const ImVec2& xy1, const ImVec2& xy2) const {
	const auto clrLearned = ImColor(193, 254, 0);

	int lvl = ability.level, maxLvl = ability.maxLevel;

	const auto elemWidth = (xy2.x - xy1.x) / maxLvl;
	auto dl = ImGui::GetBackgroundDrawList();
	const ImVec2 outline{ 1.f,1.f };
	for (int i = 0; i < lvl; ++i) {
		auto barXY1 = ImVec2(xy1.x + i * elemWidth, xy1.y);
		auto barXY2 = ImVec2(xy1.x + (i + 1) * elemWidth, xy2.y);
		dl->AddRectFilled(
			barXY1,
			barXY2,
			ImColor{ 0,0,0 },
			elemWidth / 4
		);
		dl->AddRectFilled(
			barXY1 + outline,
			barXY2 - outline,
			clrLearned,
			elemWidth / 4
		);
	}
}