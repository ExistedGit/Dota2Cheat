#pragma once
#include "../pch.h"
#include "../../Utils/Drawing.h"
#include "../../CheatSDK/include.h"

#include "../MListeners.h"

namespace Modules {
	// Displays their HP as a number
	// The manabar feature was integrated into Dota itself!
	inline class M_BarAugmenter : public IFrameListener, public IEntityListListener {
		bool CanDraw(CHero* hero) const;

		struct RenderData {
			bool drawable;
			ImVec2 pos;
			unsigned health;
		};

		std::unordered_map<entidx_t, RenderData> renderData;
	public:
		void Draw() {
			if (!Config::Bars::HPNumbers) return;

			for (const auto& [idx, data] : renderData) {
				if (!data.drawable) continue;
				const int fontSize = 14;

				ImVec2 drawPos = data.pos;

				drawPos.y -= 34;

				ImDrawText(DrawData.GetFont("Monofonto", fontSize), std::to_string(data.health), drawPos, fontSize, ImColor{ 255,255,255 }, true);
			}

		}

		void OnFrame() override;

		void OnEntityAdded(const EntityWrapper& ent) override;
		void OnEntityRemoved(const EntityWrapper& ent) override;

		M_BarAugmenter() : IFrameListener() {
			IEntityListListener::Subscribe(this);
		};
	} BarAugmenter{};
}