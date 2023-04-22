#pragma once
#include "../../SDK/pch.h"
#include "../../Config.h"
#include "../../Input.h"
#include "../../Utils/Drawing.h"
#include <regex>

#include "../../ImGuiSDK/Elements.h"

namespace Pages {
	namespace AutoPickHeroGrid {
		inline std::vector < std::string> heroNames;
		inline std::map<std::string, ImTextureID> heroIcons;
		inline bool Initialized = false;

		void InitList();
		void InitImages();
		// Button toggles
		inline bool pickToggled = true, banToggled = false;
		void Draw();
	}
}