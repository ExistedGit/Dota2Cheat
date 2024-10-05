#pragma once
#include "../../../pch.h"
#include "../../../CheatSDK/include.h"
#include <map>

namespace Modules {
	inline class M_AbilityESP {

		enum class LevelCounterType {
			Bars,            // Green bars similar to Dota's HUD
			NumberBasic,     // Relatively big numbers with no decorations
			NumberImmersive  // Dota-style elements
		};

		enum class ItemPanelType {
			Sequence,
			Grid
		};

		// Scales a value according to the config parameter
		template<typename T = int>
		T ScaleVar(auto val) const {
			return (T)(val * Config::AbilityESP::UIScale);
		}

		int AbilityIconSize = 32;

		bool CanDraw(const CHero* hero);

		void DrawHeroAbilities(const CHero* hero);
		void DrawHeroItems(const CHero* hero);

		// ItemESP modes
		void DrawItemSequences(const CHero* hero);
		void DrawItemGrids(const CHero* hero);

		void DrawItemIcon(const CItem* item, const ImVec2& pos, const ImVec2& size);

		void DrawItemCircle(const CItem* item, const ImVec2& xy1, const ImVec2& xy2, const ImVec2& iconSize, const int radius);

		// Level display modes
		void DrawLevelCounterImmersive(const CAbility* ability, const ImVec2& pos);
		void DrawLevelCounterBasic(const CAbility* ability, const ImVec2& pos);
		void DrawLevelBars(const CAbility* ability, const ImVec2& xy1, const ImVec2& xy2);

		void DrawChargeCounter(int charges, const ImVec2& pos) {
			auto lvlCounterType = (LevelCounterType)Config::AbilityESP::LevelCounterType;
			switch (lvlCounterType) {
			case LevelCounterType::NumberImmersive:
				DrawChargeCounterImmersive(charges, pos);
				break;
			default:
				DrawChargeCounterBasic(charges, pos);
				break;
			}
		}

		void DrawChargeCounterImmersive(int charges, const ImVec2& pos);

		void DrawChargeCounterBasic(int charges, const ImVec2& pos);
	public:
		void Draw();
	} AbilityESP;
}