#pragma once
#include "../../pch.h"
#include "../../Utils/Drawing.h"
#include "../../CheatSDK/include.h"
#include "MultiThreadModule.h"
#include "../MListeners.h"

using namespace Panorama;

namespace Modules {
	// Augments Dota's own UI built on Panorama
	// We only use Panorama's functions to obtain data on where and what to draw with ImGui
	inline class UIOverhaul : public MultiThreadModule, public IFrameListener {
		// The slope is 2-way
		static constexpr ImVec2 topBarImgSize{ 66 - 4, 36 };
		static constexpr int topBarImgSlope = 4;
		
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

		CUIPanel* GetTopBarImgForHero(CDOTABaseNPC_Hero* hero);

		void UpdateHeroes();
		bool NWPanelStateQueued;
		void UpdateNetworthPanel();
	public:
		void QueueUpdateNetworthPanel() {
			MTM_LOCK;
			NWPanelStateQueued = true;
		}

		// Mana and Health bars
		void DrawBars();
		void Init();

		void OnFrame() override {
			MTM_LOCK;
			UpdateNetworthPanel();

			if (!Config::UIOverhaul::TopBars)
				return;

			bool needsUpdate = false;
			EntityList.ForEach<CDOTABaseNPC_Hero>([this, &needsUpdate](auto* hero) {
				if (needsUpdate || hero->IsIllusion() || hero->IsSameTeam(ctx.localHero) || topBar.contains(hero))
				return;
			needsUpdate = true;
				});

			if (needsUpdate)
				UpdateHeroes();
		}
		void Reset() {
			MTM_LOCK;

			topBar.clear();
		}
	} UIOverhaul;
}