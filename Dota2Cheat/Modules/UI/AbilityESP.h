#pragma once
#include "../pch.h"
#include "../../Utils/Drawing.h"
#include "../../CheatSDK/include.h"
#include "MultiThreadModule.h"
#include <map>

namespace Modules {
	inline class M_AbilityESP : public MultiThreadModule {
		struct AbilityData {
			CDOTABaseAbility* ability{};
			ID3D11ShaderResourceView* icon{};
		};

		enum class LevelCounterType {
			Bars,
			Number
		};

		enum class ItemPanelType {
			Sequence,
			Grid
		};

		// Scales a value according to the config parameter
		template<typename T = int>
		T ScaleVar(auto val) {
			return (T)(val * Config::AbilityESP::UIScale);
		}

		int AbilityIconSize = 32;
		bool Initialized = false;
		std::map<CDOTABaseNPC_Hero*, std::vector<AbilityData>> EnemyAbilities{};
		// For each hero there's a map of slot indexes to ability data(for items tho, but they're abilities too)
		std::map<CDOTABaseNPC_Hero*, std::map<int, AbilityData>> EnemyItems{};

		std::map<CDOTABaseNPC_Hero*, bool> DrawableHeroes;
		bool CanDraw(CDOTABaseNPC_Hero* hero);;
		void DrawAbilities();
		void LoadItemTexIfNeeded(AbilityData& data);

		// ItemESP modes
		void DrawItemSequences();
		void DrawItemGrids();;
		void DrawItemIcon(std::map<int, AbilityData>& inv, int slot, const ImVec2& pos, const ImVec2& size);
		// void DrawItemGrids();

		void DrawItemCircle(const AbilityData& data, const ImVec2& xy1, const ImVec2& xy2, const ImVec2& iconSize, const int radius);

		// Level display modes
		void DrawLevelCounter(CDOTABaseAbility* ability, const ImVec2& pos);
		void DrawLevelBars(CDOTABaseAbility* ability, const ImVec2& xy1, const ImVec2& xy2);

		void DrawChargeCounter(int charges, const ImVec2& pos, int radius);

		void UpdateAbilities(CDOTABaseNPC_Hero* hero);
		void UpdateItems(CDOTABaseNPC_Hero* hero);
	public:
		void UpdateHeroData();
		void SubscribeHeroes();
		void Reset();
		void DrawESP();
	} AbilityESP;
}