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

		struct HeroItemData {
			std::vector<AbilityData> mainItems;
			std::vector<AbilityData> stash;
			AbilityData neutral{};
			AbilityData tp{};
			HeroItemData() {
				mainItems.reserve(6);
				stash.reserve(3);
				for (int i = 0; i < 6; ++i)
					mainItems.push_back(AbilityData());
				for (int i = 0; i < 3; ++i)
					stash.push_back(AbilityData());
			}
		};

		std::map<std::string, TextureData*> loadingQueue;

		// Scales a value according to the config parameter
		template<typename T = int>
		T ScaleVar(auto val) {
			return (T)(val * Config::AbilityESP::UIScale);
		}

		int AbilityIconSize = 40;
		bool Initialized = false;
		static inline std::map<CDOTABaseNPC_Hero*, std::vector<AbilityData>> EnemyAbilities{};
		static inline std::map<CDOTABaseNPC_Hero*, HeroItemData> EnemyItems{};

		bool CanDraw(CDOTABaseNPC_Hero* hero);;
		void DrawAbilities(ImFont* textFont);
		void DrawItems(ImFont* textFont);
		void DrawLevelCounter(CDOTABaseAbility* ability, ImFont* font, ImVec2 pos);
		void DrawChargeCounter(CDOTABaseAbility* ability, ImFont* font, ImVec2 pos, int radius);
	public:
		void UpdateAbilities(CDOTABaseNPC_Hero* hero);
		void UpdateItems(CDOTABaseNPC_Hero* hero);
		void UpdateHeroData();
		void SubscribeHeroes();
		void Reset();
		void DrawESP(ImFont* textFont);
	};
}
namespace Modules {
	inline ESP::AbilityESP AbilityESP{};
}