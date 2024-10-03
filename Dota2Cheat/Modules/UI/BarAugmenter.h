#pragma once
#include "../pch.h"
#include "../../Utils/Drawing.h"
#include "../../CheatSDK/include.h"

namespace Modules {
	// Displays their HP as a number
	// The manabar feature was integrated into Dota itself!
	inline class M_BarAugmenter {
		bool CanDraw(CHero* hero);
		void DrawHPNumbers(CHero* hero) {
			const int fontSize = 14;
			if (!CanDraw(hero))
				return;

			ImVec2 drawPos =
				HeroData[hero].HealthbarW2S;

			drawPos.y -= 34;

			DrawTextForeground(DrawData.GetFont("Monofonto", fontSize), std::to_string(hero->GetHealth()), drawPos, fontSize, ImColor{ 255,255,255 }, true);
		}
	public:
		void Init() {}
		void Draw() {
			if (Config::Bars::HPNumbers)
				EntityList.ForEach<CHero>([this](auto* hero) { DrawHPNumbers(hero); });
		}
	} BarAugmenter{};
}