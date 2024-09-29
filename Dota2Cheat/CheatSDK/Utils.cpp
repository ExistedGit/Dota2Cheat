#include "Utils.h"

// Rebuilt from xref "Minimap Objects", that formula is all over the CDOTAPanoramaMinimapRenderer::RenderX methods
ImVec2 WorldToMap(const Vector& EntityPos) {
	static auto minimap = Panorama::DotaHud->FindChildByIdTraverse("minimap");
	if (!minimap) return { 0, 0 };

	static auto mmr = minimap->Member<CDOTAPanoramaMinimapRenderer*>(0x28);
	if (!mmr) return { 0,0 };

	auto bounds = mmr->MinimapBounds;
	auto mms = mmr->GetMinimapSize();
	auto quotient = mmr->Member<float>(0x10 + 0xC);
	float 
		xAbs = (EntityPos.x - mmr->Member<float>(0x10)) / quotient, 
		yAbs = (EntityPos.y - mmr->Member<float>(0x14)) * (-1.0 / quotient);

	if (xAbs >= 75.0f)
		xAbs = fminf(xAbs, 949.0f);
	else
		xAbs = 75.0f;
	if (yAbs >= 75.0)
		yAbs = fminf(yAbs, 949.0f);
	else
		yAbs = 75.0f;

	float v13 = (quotient * xAbs) + bounds.x;
	float quotient2 = mmr->Member<float>(0x10 + 0x10) * 0.0009765625;
	float v15 = bounds.y - (quotient * yAbs);

	auto a1 = (uintptr_t)mmr + 0x10;

	float x = ((((xAbs - 512.0) * quotient2) + 0.5) * (*(uint32_t*)(a1 + 0x9C) - *(uint32_t*)(a1 + 0xA4) - *(uint32_t*)(a1 + 0xAC))) + (float)*(int*)(a1 + 0xA4);
	float y = ((((yAbs - 512.0) * quotient2) + 0.5) * (*(uint32_t*)(a1 + 0xA0) - *(uint32_t*)(a1 + 0xB0) - *(uint32_t*)(a1 + 0xA8))) + (float)*(int*)(a1 + 0xA8);

	auto base = minimap->GetPanel2D()->GetPositionWithinWindow();
	return base + Vector2D{ x, y };
}