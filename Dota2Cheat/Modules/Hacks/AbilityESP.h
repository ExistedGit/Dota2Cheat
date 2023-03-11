#pragma once
#include "../SDK/pch.h"
#include "../../Utils/Drawing.h"
#include "../../Config.h"
#include <map>
#include <ShlObj_core.h>

namespace Config {
#ifdef _DEBUG
	inline bool AbilityESPEnabled = true;
#else
	inline bool AbilityESPEnabled = false;
#endif
	inline bool AbilityESPShowAllies = true;
	inline float AbilityESP_UIScale = 1.0f;
}

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
		template<typename T>
		T ScaleVar(auto val) {
			return (T)(val * Config::AbilityESP_UIScale);
		}

		int DefaultIconSize = 40;
		bool Initialized = false;
		static inline std::map<CDOTABaseNPC_Hero*, std::vector<AbilityData>> EnemyAbilities{};
		std::string assetsPath;

		void DrawAbilities(ImFont* textFont);
		void DrawLevelCounter(CDOTABaseAbility* ability, ImFont* font, ImVec2 pos);
		void DrawChargeCounter(CDOTABaseAbility* ability, ImFont* font, ImVec2 pos, int radius);
	public:
		AbilityESP() {

			char buf[256];
			SHGetSpecialFolderPathA(0, buf, CSIDL_PROFILE, false);
			assetsPath = buf;
			assetsPath += R"(\Documents\Dota2Cheat\assets)";
		}
		void UpdateAbilities();
		void SubscribeHeroes();
		void Reset();
		void DrawESP(ImFont* textFont);
	};
}
namespace Modules {
	inline ESP::AbilityESP AbilityESP{};
}