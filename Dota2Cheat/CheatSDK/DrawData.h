#pragma once
#include <imgui/imgui.h>
#include <string>
#include <map>
inline struct _DrawData {
	// some people love to play with their Dota HUD flipped
	bool IsHUDFlipped = false;
	std::map<std::string, std::map<int, ImFont*>> Fonts;

	// Clamps the size to 10-26 range and rounds the font size to the nearest even number
	ImFont* GetFont(const std::string& name, int size) {
		size = std::clamp(size, 10, 26);
		if (size % 2 != 0)
			++size;

		return Fonts[name][size];
	}

} DrawData;