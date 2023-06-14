#pragma once
#include "../../SDK/pch.h"
#include "../../Utils/Drawing.h"
#include "../../CheatSDK/include.h"
#include "MultiThreadModule.h"

using namespace Panorama;

namespace Hacks {
	// Augments Dota's own UI built on Panorama
	// We only use Panorama's functions to obtain data on where and what to draw with ImGui
	class UIOverhaul : public MultiThreadModule {
		// The slope is 2-way
		static constexpr ImVec2 topBarImgSize{ 66 - 4, 36 };
		static constexpr int topBarImgSlope = 4;
		uint16_t TopBarClass{};

		struct TopBarImgData {
			ImVec2 imgPos{};
			CUIPanel* panel;
			bool IsDire; // Used for calculating the slope
			TopBarImgData() {}
			TopBarImgData(CUIPanel* panel) : panel(panel) {
				// DOTATopBarPlayer is either RadiantPlayerN or DirePlayerN
				std::string_view id = panel->GetParent()->GetParent()->GetId();
				IsDire = id.starts_with("Dire");
				imgPos = ImVecFromVec2D(panel->GetPanel2D()->GetPositionWithinWindow());
			}
		};

		// Top bar images linked with the heroes they are for
		std::map<CDOTABaseNPC_Hero*, TopBarImgData> topBar;
		CDOTABaseNPC_Hero* FindHeroByUnitName(std::string_view name);

		CUIPanel* GetTopBarImgForHero(CDOTABaseNPC_Hero* hero);

		void UpdateHeroes();
	public:
		// Mana and Health bars
		void DrawBars();
		void Init();
		void Update() {
			if (!Config::UIOverhaul::TopBars)
				return;

			MTM_LOCK;

			for (auto& hero : ctx.heroes) {
				if (hero->IsIllusion() || hero->IsSameTeam(ctx.localHero) || topBar.contains(hero))
					continue;
				UpdateHeroes();
				break;
			}
		}
		void Reset() {
			MTM_LOCK;

			topBar.clear();
		}
	};
}
namespace Modules {
	inline Hacks::UIOverhaul UIOverhaul{};
}