#include "Utils.h"

ImVec2 WorldToMap(const Vector& EntityPos) {
	auto mmr = GameSystems::MinimapRenderer;

	if (!mmr)
		return { 0,0 };
	static auto dota_hud_flip = Interfaces::CVar->CVars["dota_hud_flip"].m_pVar;
	const auto ScreenSize = Panorama::DotaHud->GetScreenSize();

	auto bounds = mmr->MinimapBounds;
	auto quotient = mmr->Member<float>(0x10 + 0xC);
	float v10 = (EntityPos.x - bounds.x) / quotient, v11 = (EntityPos.y - bounds.y) * (-1.0 / quotient);

	if (v10 >= 75.0f)
		v10 = fminf(v10, 949.0f);
	else
		v10 = 75.0f;
	if (v11 >= 75.0)
		v11 = fminf(v11, 949.0f);
	else
		v11 = 75.0f;

	float v13 = (quotient * v10) + bounds.x;
	float v14 = mmr->Member<float>(0x10 + 0x10) * 0.0009765625;
	float v15 = bounds.y - (quotient * v11);
	float v16 = ((mmr->MinimapSizeX - mmr->Member<int>(0xA4) - mmr->Member<uint32_t>(0xAC)) * (((v10 - 512.0) * v14) + 0.5)) + mmr->Member<int>(0xA4);
	float v17 = ((mmr->MinimapSizeY - mmr->Member<int>(0xA8) - mmr->Member<int>(0xB0)) * (((v11 - 512.0) * v14) + 0.5)) + mmr->Member<int>(0xA8);


	// The border around the actual map panel is around 12px
	auto ActualMinimapSize = static_cast<float>(mmr->GetMinimapSize().x * 0.94);
	auto MinimapPosMin = Vector2D(0, static_cast<float>(ScreenSize.y - ActualMinimapSize));

	if (dota_hud_flip->value.boolean) {
		float offset = ScreenSize.x - ActualMinimapSize;
		MinimapPosMin.x = MinimapPosMin.x + offset;
	}

	Vector2D Scaler = mmr->MinimapBounds / ActualMinimapSize * 2;
	auto PosOnMinimap = MinimapPosMin + (mmr->MinimapBounds - Vector2D{ EntityPos.x, EntityPos.y }) / Scaler;

	return PosOnMinimap;
}