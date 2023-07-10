#pragma once
#include "../SDK/pch.h"
#include "../../Utils/Drawing.h"
#include "../../CheatSDK/include.h"

namespace Modules {
	// Adds manabars to enemies and displays their HP as a number
	inline class BarAugmenter {
		bool CanDraw(CDOTABaseNPC_Hero* hero);
		void DrawHPNumbers();
		void DrawManabars();
	public:
		void Draw() {
			if (Config::Bars::ManaBar)
				DrawManabars();
			if (Config::Bars::HPNumbers)
				DrawHPNumbers();
		}
	} BarAugmenter{};
}