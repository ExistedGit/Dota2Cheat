#include "SpeedIndicator.h"

void Hacks::SpeedIndicator::Draw() {
	if (!Config::Indicators::Speed)
		return;

	const auto icon = (void*)texManager.GetNamedTexture("icon_speed");
	int mySpeed = ctx.assignedHero->GetIdealSpeed();
	for (auto& hero : ctx.heroes) {
		if (hero->IsSameTeam(ctx.assignedHero)
			|| !hero->IsTargetable()
			|| hero->IsIllusion())
			continue;

		auto heroPos = hero->GetPos();
		heroPos.z += hero->Member<int>(Netvars::C_DOTA_BaseNPC::m_iHealthBarOffset);

		auto barPos = WorldToScreen(heroPos);

		int enemySpeed = hero->GetIdealSpeed();

		auto drawPos = barPos - ImVec2{ 105,38 };
		ImColor img_color = { 255, 255, 255, 255 };
		if (enemySpeed > mySpeed)
			img_color = { 255, 0, 0, 255 };
		else if (enemySpeed < mySpeed)
			img_color = { 0, 255, 0, 255 };

		ImGui::GetForegroundDrawList()->AddImage(
			icon,
			drawPos + ImVec2{ 2, 2 },
			drawPos + ImVec2(25, 25),
			{ 0.0f, 0.0f },
			{ 1.0f, 1.0f },
			img_color);
	}
}
