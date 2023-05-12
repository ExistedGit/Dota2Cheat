#include "HookIndicator.h"

Vector Hacks::HookIndicator::CalculateHook(CDOTABaseNPC* enemy) {
	Vector myPos = ctx.localHero->GetPos(),
		enemyPos = enemy->GetPos();

	return myPos + (((enemyPos - myPos) * enemy->GetIdealSpeed()) / (enemy->GetIdealSpeed() * 1450)) * 1450;
}

void Hacks::HookIndicator::Draw() {
	constexpr ImVec2 iconSize{ 16,16 };
	for (auto& hero : ctx.heroes) {
		if (hero->IsSameTeam(ctx.localHero)
			|| !hero->IsTargetable()
			|| hero->IsIllusion()
			|| !IsEntityOnScreen(hero))
			continue;

		Vector hookPos = hero->IsMoving() ? CalculateHook(hero) : hero->GetPos();
		auto drawPos = WorldToScreen(hookPos);
		auto DrawList = ImGui::GetForegroundDrawList();
		DrawList->AddRectFilled(drawPos - iconSize / 2, drawPos + iconSize / 2, ImColor{ 255,0,0 });
	}
}
