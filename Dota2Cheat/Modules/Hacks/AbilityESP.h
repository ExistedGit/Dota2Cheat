#pragma once
#include "../SDK/pch.h"
#include "../../Utils/Drawing.h"
#include "../../Config.h"
#include <map>

namespace Config {
	inline bool AbilityESPEnabled = false;
	inline bool AbilityESPShowAllies = true;
	inline float AbilityESP_UIScale = 1.0f;
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
		
		// Scales a value according to the config parameter
		template<typename T>
		T ScaleVar(auto val) {
			return (T)(val * Config::AbilityESP_UIScale);
		}

		int DefaultIconSize = 40;
		bool Initialized = false;
		static inline std::map<CDOTABaseNPC_Hero*, std::vector<AbilityData>> EnemyAbilities{};
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