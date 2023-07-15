#pragma once
#include <d3d11.h>
#include "../../pch.h"
#include "../../CheatSDK/include.h"
#include "../../Utils/Drawing.h"

#include "../../ImGuiSDK/Elements.h"

namespace Pages {
	namespace AutoPickHeroGrid {
		inline std::vector < std::string> heroNames;
		inline std::map<std::string, ID3D11ShaderResourceView*> heroIcons;
		inline bool Initialized = false;

		void InitList();
		// Button toggles
		inline bool pickToggled = true, banToggled = false;
		void Draw();
	}
}