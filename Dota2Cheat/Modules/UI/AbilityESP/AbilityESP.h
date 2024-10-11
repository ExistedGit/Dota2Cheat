#pragma once
#include "../../../pch.h"
#include "../../../CheatSDK/include.h"
#include "../../MListeners.h"
#include "../MultiThreadModule.h"
#include <map>

namespace Modules {
	inline class M_AbilityESP : public IFrameListener, public MultiThreadModule, public IEntityListListener {

		enum class LevelCounterType {
			Bars,            // Green bars similar to Dota's HUD
			NumberBasic,     // Relatively big numbers with no decorations
			NumberImmersive  // Dota-style elements
		};

		enum class ItemPanelType {
			Sequence,
			Grid
		};

		struct RenderData {
			ImVec2 HealthbarW2S, W2S;
			bool
				isDrawable,
				hasStatus; // has overhead status effect, need visual adjustment

			struct Ability {
				const char* icon;
				int level;
				int maxLevel;
				std::optional<int> charges;
				float cooldown;

				bool toggled;
				bool autocast;
				bool noMana;

				float castTime = 0;
				float castPoint = 0;

				float channelingTime = 0;
				float channelingTimeTotal = 0;
			} abilities[6];

			int abilityCount;

			struct Item {
				std::string icon;
				// The item array represents the physical layout(used e. g. in Grid display mode),
				// thus we need to be able to tell that a slot is empty
				bool valid;
				bool toggled;
				std::optional<int> charges;
				float cooldown;
			};

			Item items[6], tpSlot, neutralSlot;
		};

		const unsigned VISIBLE_ABILITY_COUNT = 6;
		const unsigned VISIBLE_ITEM_COUNT = 6;

		// uint32_t = entity ID
		std::unordered_map<uint32_t, RenderData> renderData;

		void UpdateHeroRenderData(const CHero*);

		void OnFrame() override {

			MTM_LOCK;

			for (auto& [heroID, _] : renderData)
				UpdateHeroRenderData(EntityList.Get<CHero>(heroID));
		}

		// Scales a value according to the config parameter
		template<typename T = int>
		T ScaleVar(auto val) const {
			return (T)(val * Config::AbilityESP::UIScale);
		}

		int AbilityIconSize = 32;

		bool CanDraw(const CHero* hero) const;

		void DrawHeroAbilities(const RenderData& rd) const;

		void DrawHeroItems(const RenderData& rd) const;

		// ItemESP modes
		// Draws the same block sequence as for abilities + two circles for TP and neutral slots on the right and left respectively
		// Only draws slots occupied by an item
		// If the item is toggled(like Armlet), a green frame is drawn
		// If the item has charges(like Magic Wand), a counter is displayed in the top left corner of the image
		void DrawItemSequences(const RenderData& rd) const;
		void DrawItemGrids(const RenderData& rd) const;

		void DrawItemIcon(const RenderData::Item& item, const ImVec2& pos, const ImVec2& size) const;

		void DrawItemCircle(const RenderData::Item& item, const ImVec2& xy1, const ImVec2& xy2, const ImVec2& iconSize, const int radius) const;

		// Level display modes
		void DrawLevelCounterImmersive(const RenderData::Ability& ability, const ImVec2& pos) const;
		void DrawLevelCounterBasic(const RenderData::Ability& ability, const ImVec2& pos) const;
		void DrawLevelBars(const RenderData::Ability&, const ImVec2& xy1, const ImVec2& xy2) const;

		void DrawChargeCounter(int charges, const ImVec2& pos) const {
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

		void DrawChargeCounterImmersive(int charges, const ImVec2& pos) const;

		void DrawChargeCounterBasic(int charges, const ImVec2& pos) const;
	public:
		void Reset() {
			renderData.clear();
		}
		void Draw();

		void OnEntityAdded(const EntityWrapper& ent) override;
		void OnEntityRemoved(const EntityWrapper& ent) override;
	} AbilityESP;
}