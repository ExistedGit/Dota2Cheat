#pragma once

enum class CheatUIState {
	LaunchMenu,
	Active
};

namespace UIData {
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