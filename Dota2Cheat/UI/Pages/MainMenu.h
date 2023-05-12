#pragma once
#include "../Utils/Drawing.h"
#include "../../CheatSDK/include.h"
#include "../../DebugFunctions.h"
#include "../../ImGuiSDK/Elements.h"
#include "../../Modules/Hacks/TargetedSpellHighlighter.h"
#include "../../Modules/Hacks/TrueSightESP.h" 

namespace Pages {
	namespace MainMenu {
		inline const char* WeatherList[] = { "Default", "Winter", "Rain", "MoonBeam", "Pestilence", "Harvest", "Sirocco", "Spring", "Ash", "Aurora" };
		inline const char* RiverList[] = { "Default", "Chrome", "Dry", "Slime", "Oil", "Electric", "Potion", "Blood" };

		inline char scriptBuf[4096]{};
		inline std::string rpStatusBuf;
		inline std::string uiSoundBuf;
		inline bool scriptMenuVisible = false;
		inline bool circleMenuVisible = false;

		inline int debugEntIdx = 0;

		void Draw();
	}
}