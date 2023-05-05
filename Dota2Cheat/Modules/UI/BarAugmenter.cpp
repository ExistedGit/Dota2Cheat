#include "BarAugmenter.h"

bool Hacks::BarAugmenter::CanDraw(CDOTABaseNPC_Hero* hero) {
	bool ret = IsValidReadPtr(hero)
		&& IsValidReadPtr(hero->GetIdentity())
		&& !hero->GetIdentity()->IsDormant()
		&& !hero->IsIllusion()
		&& hero != ctx.localHero
		&& hero->GetLifeState() == 0
		&& IsEntityOnScreen(hero);

	return ret;
}

void Hacks::BarAugmenter::DrawHPNumbers() {
	constexpr static ImVec2 manabarSize{ 101, 8 };
	const int fontSize = 14;
	for (auto& hero : ctx.heroes) {
		if (!CanDraw(hero))
			continue;
		if (hero->IsSameTeam(ctx.localHero))
			continue;


		int hbo = hero->Member<int>(Netvars::C_DOTA_BaseNPC::m_iHealthBarOffset);

		Vector pos = hero->IsMoving()
			? pos = hero->GetForwardVector(hero->GetMoveSpeed() * (-0.0167f)) // Going back 1 frame to synchronize with the game
			: pos = hero->GetPos();
		pos.z += hbo;

		ImVec2 drawPos = WorldToScreen(pos);
		drawPos.y -= 32;

		DrawTextForeground(DrawData.GetFont("Monofonto", fontSize), std::to_string(hero->GetHealth()), drawPos, fontSize, ImColor{ 255,255,255 }, true);
	}
}

void Hacks::BarAugmenter::DrawManabars()
{
	// Fine-tuned values
	// idk why it's this strange
	constexpr static ImVec2 manabarSize{ 101, 6 };
	for (auto& hero : ctx.heroes) {
		if (!CanDraw(hero))
			continue;
		if (hero->IsSameTeam(ctx.localHero))
			continue;

		int hbo = hero->Member<int>(Netvars::C_DOTA_BaseNPC::m_iHealthBarOffset);

		Vector pos = hero->IsMoving()
			? pos = hero->GetForwardVector(hero->GetMoveSpeed() * (-0.0167f)) // Going back 1 frame to synchronize with the game
			: pos = hero->GetPos();
		pos.z += hbo;

		ImVec2 drawPos = WorldToScreen(pos) + ImVec2(4, -16);
		// Background
		DrawRectFilled(
			drawPos - ImVec2(110, manabarSize.y) / 2,
			manabarSize, ImVec4(0, 0, 0, 1));
		// Manabar
		DrawRectFilled(
			drawPos - ImVec2(110, manabarSize.y) / 2 + ImVec2(1, 1),
			ImVec2(manabarSize.x * (hero->GetMana() / hero->GetMaxMana()) - 2, manabarSize.y - 2), ImVec4(0, 0.5, 1, 1));
	}

}
