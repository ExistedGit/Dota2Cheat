#pragma once
#include "../SDK/pch.h"
#include "../../Utils/Drawing.h"
#include "../../CheatSDK/include.h"
#include "MultiThreadModule.h"
#include <map>

namespace ESP {
	class AbilityESP : public MultiThreadModule {
		struct AbilityData {
			CDOTABaseAbility* ability{};
			ImTextureID icon{};
		};
		

		// Scales a value according to the config parameter
		template<typename T = int>
		T ScaleVar(auto val) {
			return (T)(val * Config::AbilityESP::UIScale);
		}

		int AbilityIconSize = 32;
		bool Initialized = false;
		static inline std::map<CDOTABaseNPC_Hero*, std::vector<AbilityData>> EnemyAbilities{};
		// For each hero there's a map of slot indexes to ability data(for items tho, but they're abilities too)
		static inline std::map<CDOTABaseNPC_Hero*, std::map<int, AbilityData>> EnemyItems{};

		std::map<CDOTABaseNPC_Hero*, bool> DrawableHeroes;
		bool CanDraw(CDOTABaseNPC_Hero* hero);;
		void DrawAbilities();
		void LoadItemTexIfNeeded(AbilityData& data);
		void DrawItems();
		void DrawItemCircle(const AbilityData& data, const ImVec2& xy1, const ImVec2& xy2, const ImVec2& iconSize, const int radius);
		void DrawLevelCounter(CDOTABaseAbility* ability, ImVec2 pos);
		void DrawLevelBars(CDOTABaseAbility* ability, const ImVec2& xy1, const ImVec2& xy2);
		void DrawChargeCounter(int charges, const ImVec2& pos, int radius);
		void DrawManabars();
		void UpdateAbilities(CDOTABaseNPC_Hero* hero);
		void UpdateItems(CDOTABaseNPC_Hero* hero);
	public:
		void UpdateHeroData();
		void SubscribeHeroes();
		void Reset();
		void DrawESP();
	};
}
namespace Modules {
	inline ESP::AbilityESP AbilityESP{};
}