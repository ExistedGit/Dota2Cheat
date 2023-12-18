#pragma once
#include "../Modules/Hacks/TreeChanger.h"


enum class CheatUIState {
	LaunchMenu,
	Active
};

namespace UIData {
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
	inline CheatUIState uiState = CheatUIState::LaunchMenu;
}