#include "Utils.h"

Vector2D::Vector2D(const CMsgVector2D& msg) {
	x = msg.has_x() ? msg.x() : 0;
	y = msg.has_y() ? msg.y() : 0;
}

Vector::Vector(const CMsgVector& msg) {
	x = msg.has_x() ? msg.x() : 0;
	y = msg.has_y() ? msg.y() : 0;
	z = msg.has_z() ? msg.z() : 0;
}

// Rebuilt from xref "Minimap Objects", that formula is all over the CDOTAPanoramaMinimapRenderer::RenderX methods
ImVec2 WorldToMap(const Vector& pos) {
	static auto minimap = Panorama::DotaHud->FindChildByIdTraverse("minimap");
	if (!minimap) {
		ONLY_ONCE{
			LogFE("{}: minimap element not found in HUD!", __FUNCTION__);
		};
		return { 0, 0 };
	}

	auto mmr = minimap->GetPanel2D()->Member<CDOTAPanoramaMinimapRenderer*>(0x28);
	if (!mmr) {
		ONLY_ONCE{
			LogFE("{}: CDOTAPanoramaMinimapRenderer not found in minimap element's CPanel2D!", __FUNCTION__);
		};
		return { 0,0 };
	}

	auto bounds = mmr->MinimapBounds;
	auto mms = mmr->GetMinimapSize();
	auto quotient = mmr->Member<float>(0x10 + 0xC);
	float
		xAbs = (pos.x - mmr->Member<float>(0x10)) / quotient,
		yAbs = (pos.y - mmr->Member<float>(0x14)) * (-1.0 / quotient);

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