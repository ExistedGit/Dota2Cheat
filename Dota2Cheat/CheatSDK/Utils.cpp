#include "Utils.h"

ImVec2 WorldToMap(const Vector& EntityPos) {
	if (!GameSystems::MinimapRenderer)
		return { 0,0 };
	const auto ScreenSize = ImVecFromVec2D(GameSystems::DotaHud->GetScreenSize());

	// The border around the actual map panel is around 12px
	auto ActualMinimapSize = static_cast<float>(GameSystems::MinimapRenderer->GetMinimapSize().x * 0.94);
	auto MinimapPosMin = Vector2D(0, static_cast<float>(ScreenSize.y - ActualMinimapSize));

	if (DrawData.IsHUDFlipped) {
		float offset = ScreenSize.x - ActualMinimapSize;
		MinimapPosMin.x = MinimapPosMin.x + offset;
	}

	Vector2D Scaler = GameSystems::MinimapRenderer->MinimapBounds / ActualMinimapSize * 2;
	auto PosOnMinimap = MinimapPosMin + (GameSystems::MinimapRenderer->MinimapBounds - Vector2D{ EntityPos.x, EntityPos.y }) / Scaler;

	return ImVecFromVec2D(PosOnMinimap);
}