#pragma once
#include <d3d11.h>
#include "../../SDK/pch.h"
#include "../../CheatSDK/Config.h"
#include "../../Utils/Drawing.h"
#include "../../CheatSDK/Systems/CheatManager.h"

#include <regex>

#include "../../ImGuiSDK/Elements.h"

namespace Pages {
	namespace AutoPickHeroGrid {
		inline std::vector < std::string> heroNames;
		inline std::map<std::string, ID3D11ShaderResourceView*> heroIcons;
		inline bool Initialized = false;

		void InitList();
		void InitImages();
		// Button toggles
		inline bool pickToggled = true, banToggled = false;
		void Draw();
	}
}