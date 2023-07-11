#pragma once
#include "../SDK/pch.h"
#include "../../Utils/Drawing.h"
#include "../../CheatSDK/include.h"

namespace Modules {
	// Adds manabars to enemies and displays their HP as a number
	inline class BarAugmenter {
		bool CanDraw(CDOTABaseNPC_Hero* hero);
		void DrawHPNumbers(CDOTABaseNPC_Hero* hero) {
			const int fontSize = 14;
			if (!CanDraw(hero))
				return;

			ImVec2 drawPos =
				HeroData[hero].HealthbarW2S;

			drawPos.y -= 32;

			DrawTextForeground(DrawData.GetFont("Monofonto", fontSize), std::to_string(hero->GetHealth()), drawPos, fontSize, ImColor{ 255,255,255 }, true);
			constexpr static ImVec2 manabarSize{ 101, 8 };
		}
		void DrawManabar(CDOTABaseNPC_Hero* hero)
		{
			// Fine-tuned values
			// idk why it's this strange
			constexpr static ImVec2 manabarSize{ 101, 6 };

				if (!CanDraw(hero))
				return;

			ImVec2 drawPos = HeroData[hero].HealthbarW2S + ImVec2(4, -16);
			// Background
			DrawRectFilled(
				drawPos - ImVec2(110, manabarSize.y) / 2,
				manabarSize, ImVec4(0, 0, 0, 1));
			// Manabar
			DrawRectFilled(
				drawPos - ImVec2(110, manabarSize.y) / 2 + ImVec2(1, 1),
				ImVec2(manabarSize.x * (hero->GetMana() / hero->GetMaxMana()) - 2, manabarSize.y - 2), ImColor(0.f, 0.5f, 1.f));
		}
	public:
		void Draw() {
			if (Config::Bars::ManaBar)
				EntityList.ForEach<CDOTABaseNPC_Hero>([this](auto* hero){ DrawManabar(hero); });
			if (Config::Bars::HPNumbers)
				EntityList.ForEach<CDOTABaseNPC_Hero>([this](auto* hero){ DrawHPNumbers(hero); });
		}
	} BarAugmenter{};
}