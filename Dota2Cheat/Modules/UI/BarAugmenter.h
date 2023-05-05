#pragma once
#include "../SDK/pch.h"
#include "../../Utils/Drawing.h"
#include "../../CheatSDK/include.h"

namespace Hacks {
	// Adds manabars to enemies and displays their HP as a number
	class BarAugmenter {
		bool CanDraw(CDOTABaseNPC_Hero* hero);
		void DrawHPNumbers();
		void DrawManabars();;
	public:
		void Draw() {
			if (Config::Bars::ManaBar)
				DrawManabars();
			if (Config::Bars::HPNumbers)
				DrawHPNumbers();
		}
	};
}
namespace Modules {
	inline Hacks::BarAugmenter BarAugmenter{};
}