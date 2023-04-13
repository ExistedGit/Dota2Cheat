#pragma once
#include "../SDK/pch.h"
#include "../../Utils/Drawing.h"

using namespace Panorama;

namespace Hacks {
	// Augments Dota's own UI built on Panorama
	// We only use Panorama's functions to obtain data on where and what to draw with ImGui
	class UIOverhaul {
		static constexpr ImVec2 topBarImgSize{ 66,36 };
		static constexpr int topBarImgSlope = 4;

		struct TopBarImgData {
			CUIPanel* panel;
			bool IsDire; // Used for calculating the slope
			TopBarImgData() {}
			TopBarImgData(CUIPanel* panel) : panel(panel) {
				std::string_view id = panel->GetParent()->GetId(); // DOTATopBarPlayer is either RadiantPlayerN or DirePlayerN
				IsDire = id.starts_with("Dire");
			}
		};

		CUIPanel* DotaHud = nullptr;
		// Top bar images linked with the heroes they are for
		std::map<CDOTABaseNPC_Hero*, TopBarImgData> topBar;
		CDOTABaseNPC_Hero* FindHeroByUnitName(std::string_view name);

		CUIPanel* GetTopBarImgForHero(CDOTABaseNPC_Hero* hero);

		void UpdateHeroes() {
			topBar.clear();
			auto topbarImages = DotaHud->FindChildrenWithClassTraverse(PClass::TopBarHeroImage);
			for (auto& panel : topbarImages) {
				if (!panel->GetId() || strcmp(panel->GetId(), "HeroImage") != 0)
					continue;

				auto heroImg = (Panorama::CDOTA_UI_HeroImage*)panel->GetPanel2D();
				//LogF(LP_DATA, "TopBarHeroImage: {} {}", (void*)panel, (void*)panel->GetPanel2D());
				if (!IsValidReadPtr(heroImg->GetSrc()))
					continue;
				std::string heroName(heroImg->GetSrc());
				heroName = heroName.substr(23, heroName.size() - 23 - 4);
				//LogF(LP_DATA, "\tHERO: {} XY: {} {}", heroName, POS.x, POS.y);
				auto hero = FindHeroByUnitName(heroName);
				if (hero)
					topBar[hero] = panel;
			}
		}
		bool Intiialized = false;
	public:
		// Mana and Health bars
		void DrawBars();
		void Init();
		void Update() {
			if (!Intiialized)
				return;

			for (auto& hero : ctx.heroes) {
				if (topBar.contains(hero))
					continue;

				UpdateHeroes();
				break;
			}
		}
		void Reset() {
			Intiialized = false;
			DotaHud = nullptr;
			topBar.clear();
		}
	};
}
namespace Modules {
	inline Hacks::UIOverhaul UIOverhaul{};
}