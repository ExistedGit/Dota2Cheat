#pragma once
#include "../SDK/pch.h"
#include "../../Utils/Drawing.h"
#include "../../Config.h"
#include <map>

namespace ESP {
	class AbilityESP {
		struct AbilityData {
			CDOTABaseAbility* ability{};
			TextureData icon{};

			float lastActiveTime{};
			float lastActiveCooldown{};
			float currentCooldown{};
		};

		// Scales a value according to the config parameter
		template<typename T = int>
		T ScaleVar(auto val) {
			return (T)(val * Config::AbilityESP::UIScale);
		}

		int DefaultIconSize = 40;
		bool Initialized = false;
		static inline std::map<CDOTABaseNPC_Hero*, std::vector<AbilityData>> EnemyAbilities{};

		void DrawAbilities(ImFont* textFont);
		void DrawLevelCounter(CDOTABaseAbility* ability, ImFont* font, ImVec2 pos);
		void DrawChargeCounter(CDOTABaseAbility* ability, ImFont* font, ImVec2 pos, int radius);
	public:
		void UpdateAbilities();
		void SubscribeHeroes();
		void Reset();
		void DrawESP(ImFont* textFont);
	};
}
namespace Modules {
	inline ESP::AbilityESP AbilityESP{};
}