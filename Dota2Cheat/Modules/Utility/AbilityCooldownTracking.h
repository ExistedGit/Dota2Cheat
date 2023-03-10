#pragma once
#include "../SDK/pch.h"
#include "../../Utils/Drawing.h"
#include "../../Config.h"
#include <map>

namespace Config {
	inline bool AbilityESPEnabled = true;
	inline bool AbilityESPShowAllies = true;
	inline float AbilityESPIconScale = 1.0f;
}

namespace ESP {
	class AbilityESP {
	public:
		struct AbilityData {
			CDOTABaseAbility* ability{};
			TextureData icon{};

			float lastActiveTime{};
			float lastActiveCooldown{};
			float currentCooldown{};
		};
		int DefaultIconSize = 40;
		bool Initialized = false;
		static inline std::map<CDOTABaseNPC*, std::vector<AbilityData>> EnemyAbilities{};
		std::string assetsPath = "C:\\Users\\user\\Documents\\Dota2Cheat\\assets\\spellicons\\";

		void SubscribeHeroes();
		void Reset();
		void UpdateAbilities();
		void DrawAbilities(ImFont* textFont);
		void FrameBasedLogic(ImFont* textFont);
	};
}
namespace Modules {
	inline ESP::AbilityESP AbilityESP{};
}