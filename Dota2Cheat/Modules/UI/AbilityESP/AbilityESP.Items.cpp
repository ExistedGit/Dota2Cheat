#include "AbilityESP.h"
#include "../../../Utils/Drawing.h"

constexpr unsigned INVENTORY_SIZE = 6;

void Modules::M_AbilityESP::DrawItemSequences(const RenderData& rd) const {
	unsigned queueSize = 0;
	for (auto& item : rd.items) {
		if (item.valid) queueSize++;
	}

	// Neutral and TP slots are rendered separately from the queue 
	// but participate in line size calculations
	int itemCount = queueSize + rd.tpSlot.valid + rd.neutralSlot.valid;

	const int gap = 1;
	const ImVec2 iconSize{ (float)ScaleVar(AbilityIconSize), (float)ScaleVar(AbilityIconSize) };

	ImVec2 basePos = rd.HealthbarW2S;
	basePos.x -= itemCount * (iconSize.x + gap) / 2;
	basePos.y -= 35 + iconSize.x;

	// Adjusting for the debuff indicator that is otherwise obscured by the panel 
	if (rd.hasStatus)
		basePos.y -= 30;

	if (rd.neutralSlot.valid) {
		ImVec2 cXY1{ basePos.x + 1 , basePos.y + 1 },
			cXY2 = cXY1 + iconSize - ImVec2{ 2,2 };
		DrawItemCircle(rd.neutralSlot, cXY1, cXY2, iconSize, (iconSize.x - 2) / 2);
		basePos.x += gap + iconSize.x;
	}

	for (int slot = 0; slot < queueSize; slot++) {
		if (!rd.items[slot].valid) continue;

		DrawItemIcon(rd.items[slot], basePos, iconSize);
		basePos.x += gap + iconSize.x;
	}

	if (rd.tpSlot.valid) {
		ImVec2 cXY1{ basePos.x + 1 , basePos.y + 1 },
			cXY2 = cXY1 + iconSize - ImVec2{ 2,2 };
		DrawItemCircle(rd.tpSlot, cXY1, cXY2, iconSize, (iconSize.x - 2) / 2);
	}
}

void Modules::M_AbilityESP::DrawItemGrids(const RenderData& rd) const {
	const ImVec2 iconSize{ ScaleVar<float>(AbilityIconSize), ScaleVar<float>(AbilityIconSize) };

	const int
		gap = 1,
		col = 3;

	auto& inv = rd.items;
	ImVec2 basePos = rd.HealthbarW2S;
	basePos.x -= col * (iconSize.x + gap) / 2;
	basePos.y -= 10;

	bool sideSlotsPresent = rd.tpSlot.valid || rd.neutralSlot.valid;
	if (sideSlotsPresent)
		basePos.x -= (iconSize.x + gap) / 2 + 4;

	ImVec2 sideSlotsBasePos = basePos;
	sideSlotsBasePos.x += col * (iconSize.x + gap) + 4;

	if (rd.tpSlot.valid) {
		ImVec2 cXY1{ sideSlotsBasePos.x + 1 , sideSlotsBasePos.y + 1 },
			cXY2 = cXY1 + iconSize - ImVec2{ 2,2 };
		DrawItemCircle(rd.tpSlot, cXY1, cXY2, iconSize, (iconSize.x - 2) / 2);
	}
	if (rd.neutralSlot.valid) {
		ImVec2 cXY1{ sideSlotsBasePos.x + 1 , sideSlotsBasePos.y + iconSize.y + 1 },
			cXY2 = cXY1 + iconSize - ImVec2{ 2,2 };
		DrawItemCircle(rd.neutralSlot, cXY1, cXY2, iconSize, (iconSize.x - 2) / 2);
	}


	int validItemsDrawn = 0;
	for (int slot = 0; slot < 6; slot++) {
		if (!inv[slot].valid)
			continue;

		int x = validItemsDrawn % col,
			y = validItemsDrawn / col;

		DrawItemIcon(inv[slot], basePos + ImVec2{ (float)x, (float)y } *(iconSize.x + gap), iconSize);
		validItemsDrawn++;
	}

}

void Modules::M_AbilityESP::DrawItemIcon(const RenderData::Item& item, const ImVec2& pos, const ImVec2& size) const {
	auto DrawList = ImGui::GetBackgroundDrawList();
	// used to convert native rectangular item images to SQUARES
	constexpr float aspectRatio = (1 - 64.f / 88) / 2;
	const float rounding = Config::AbilityESP::Rounding / 100.f * size.x / 2;

	if (!item.valid)
		return;

	ImVec2 imgXY1 = pos + ImVec2{ 1,1 },
		imgXY2 = imgXY1 + size - ImVec2{ 2,2 },
		imgCenter = (imgXY1 + imgXY2) / 2;

	ImVec2 frameSize(1, 1);
	ImVec2 frameXY1 = imgXY1 - frameSize,
		frameXY2 = imgXY2 + frameSize;

	ImU32 frameColor = ImColor{ 0,0,0,255 };
	DrawList->AddImageRounded(
		assets.items.Load(item.icon),
		imgXY1,
		imgXY2,
		ImVec2(aspectRatio, 0),
		ImVec2(1 - aspectRatio, 1),
		ImColor{ 255,255,255 },
		rounding);

	if (item.toggled)
		frameColor = ImColor(0x3, 0xAC, 0x13);

	// Frame
	DrawList->AddRect(frameXY1, frameXY2, frameColor, rounding);

	float cd = item.cooldown;

	if (cd != 0) {
		DrawList->AddRectFilled(imgXY1, imgXY2, ImColor(0, 0, 0, 130), rounding);
		auto fontSize = size.y - ScaleVar<float>(2);
		bool decimals = Config::AbilityESP::ShowCooldownDecimals;
		if (cd >= 100) {
			fontSize -= 4;
			decimals = false;
		}

		if (decimals)
			fontSize -= 4;

		DrawText(
			DrawData.GetFont("Monofonto", fontSize),
			std::vformat(Config::AbilityESP::ShowCooldownDecimals ? "{:.1f}" : "{:.0f}", std::make_format_args(cd)),
			ImVec2(imgCenter.x, imgCenter.y - fontSize / 2),
			fontSize,
			ImVec4(1, 1, 1, 1),
			true);
	}

	if (item.charges)
		DrawChargeCounter(*item.charges, frameXY1);
}

void Modules::M_AbilityESP::DrawItemCircle(const RenderData::Item& item, const ImVec2& xy1, const ImVec2& xy2, const ImVec2& iconSize, const int radius) const {
	auto DrawList = ImGui::GetBackgroundDrawList();
	const ImVec2 center = (xy1 + xy2) / 2;
	constexpr float aspectRatio = (1 - 64.f / 88) / 2;

	DrawList->AddCircleFilled(center, radius + 2, ImColor(0, 0, 0, 255));

	if (!item.valid) return;

	DrawList->AddImageRounded(
		assets.items.Load(item.icon),
		xy1,
		xy2,
		ImVec2(aspectRatio, 0),
		ImVec2(1 - aspectRatio, 1),
		ImGui::GetColorU32(ImVec4(1, 1, 1, 1)),
		radius);

	float cd = item.cooldown;
	if (cd != 0) {
		int cdFontSize = radius * 2 - 2;
		if (Config::AbilityESP::ShowCooldownDecimals)
			cdFontSize -= 4;
		// Darkens the picture
		DrawList->AddCircleFilled(center, radius, ImColor(0, 0, 0, 130));
		// Draws the cooldown
		DrawText(DrawData.GetFont("Monofonto", cdFontSize),
			std::vformat(Config::AbilityESP::ShowCooldownDecimals ? "{:.1f}" : "{:.0f}", std::make_format_args(cd)),
			ImVec2(center.x, center.y - cdFontSize / 2),
			cdFontSize,
			ImVec4(1, 1, 1, 1),
			true);
	}

	if (item.charges)
		DrawChargeCounter(*item.charges, xy1);
}

