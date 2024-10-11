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

		struct RenderData {
			ImVec2 pos{};
			bool isDire; // Used for calculating the slope
			bool isDormant;
			float health, mana;
			float healthMax, manaMax;
		};

		bool NWPanelStateQueued;
		void UpdateNetworthPanel();

		struct CDOTA_UI_Top_Bar_Player : public VClass {
			GETTER(CHandle<CHero>, GetHero, 0xC4);
		};

		std::vector<RenderData> renderQueue;

		std::vector<CUIPanel*> panels;
	public:
		void QueueUpdateNetworthPanel() {
			MTM_LOCK;
			NWPanelStateQueued = true;
		}

		// Mana and Health bars
		void Draw();
		void Init();

		void OnFrame() override;
		void Reset() {
			MTM_LOCK;

			panels.clear();
			renderQueue.clear();
		}
	} UIOverhaul;
}