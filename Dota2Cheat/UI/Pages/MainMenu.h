#pragma once
#include "../Utils/Drawing.h"
#include "../../CheatSDK/include.h"
#include "../../DebugFunctions.h"
#include "../../ImGuiSDK/Elements.h"
#include "../../Modules/Hacks/TargetedSpellHighlighter.h"
#include "../../Modules/Hacks/TrueSightESP.h" 
#include "../../Modules/Hacks/TreeChanger.h" 

namespace Pages {
	namespace MainMenu {
		inline const char* WeatherList[] = { "Default", "Winter", "Rain", "MoonBeam", "Pestilence", "Harvest", "Sirocco", "Spring", "Ash", "Aurora" };
		inline const char* RiverList[] = { "Default", "Chrome", "Dry", "Slime", "Oil", "Electric", "Potion", "Blood" };

		inline Modules::M_TreeChanger::TreeModelInfo TreeModelList[] = {
			{ "models/props_tree/newbloom_tree.vmdl", 1.0f },
			{ "models/props_tree/mango_tree.vmdl", 1.0f },
			{ "maps/journey_assets/props/trees/journey_armandpine/journey_armandpine_02_stump.vmdl", 4.5f },
			{ "models/props_tree/frostivus_tree.vmdl", 0.85f },
			{ "models/props_tree/ti7/ggbranch.vmdl", 1.0f },
		};

		inline const char* TreeNameList[] = {
			"Default",
			"New Bloom",
			"Mango",
			"Stumps",
			"Frostivus",
			"GG Branch",
		};

		inline char scriptBuf[4096]{};
		inline std::string rpStatusBuf;
		inline std::string uiSoundBuf;
		inline bool scriptMenuVisible = false;
		inline bool circleMenuVisible = false;

		inline int debugEntIdx = 0;

		void Draw();
	}
}