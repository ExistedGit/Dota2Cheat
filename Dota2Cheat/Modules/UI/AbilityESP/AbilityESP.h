#pragma once
#include "../../../pch.h"
#include "../../../Utils/Drawing.h"
#include "../../../CheatSDK/include.h"
#include <map>

namespace Modules {
	inline class M_AbilityESP {

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

		bool CanDraw(CDOTABaseNPC_Hero* hero);;

		void DrawHeroAbilities(CHero* hero);
		void DrawHeroItems(CHero* hero);;

		// ItemESP modes
		void DrawItemSequences(CHero* hero);
		void DrawItemGrids(CHero* hero);

		void DrawItemIcon(CItem* item, const ImVec2& pos, const ImVec2& size);

		void DrawItemCircle(CItem* item, const ImVec2& xy1, const ImVec2& xy2, const ImVec2& iconSize, const int radius);

		// Level display modes
		void DrawLevelCounter(CDOTABaseAbility* ability, const ImVec2& pos);
		void DrawLevelBars(CDOTABaseAbility* ability, const ImVec2& xy1, const ImVec2& xy2);

		void DrawChargeCounter(int charges, const ImVec2& pos, int radius);
	public:
		void DrawESP();
	} AbilityESP;
}