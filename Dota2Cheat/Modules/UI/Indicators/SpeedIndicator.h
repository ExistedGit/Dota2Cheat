#pragma once
#include "../../../CheatSDK/include.h"
#include "../../../Utils/Drawing.h"
#include "../../MListeners.h"
#include "../MultiThreadModule.h"

namespace Modules {
	// Created together with Wolf49406
	inline class M_SpeedIndicator : public MultiThreadModule, public IFrameListener, public IEntityListListener {
		struct RenderData {
			ImVec2 pos;
			bool drawable = false;
			int8_t relativeSpeed;
		};

		std::map<uint32, RenderData> renderData;
	public:
		void Draw() {
			if (!Config::Indicators::Speed)
				return;

			MTM_LOCK;

			const static auto icon = assets["panorama/images/hud/reborn"].Load("icon_speed", "psd");

			for (const auto& [idx, data] : renderData) {
				if (!data.drawable) continue;

				auto drawPos = data.pos - ImVec2{ 105,38 };
				ImColor img_color = { 255, 255, 255, 255 };
				if (data.relativeSpeed == -1)
					img_color = { 255, 0, 0, 255 };
				else if (data.relativeSpeed == 1)
					img_color = { 0, 255, 0, 255 };

				ImGui::GetBackgroundDrawList()->AddImage(
					icon,
					drawPos + ImVec2{ 2, 2 },
					drawPos + ImVec2(25, 25),
					{ 0.0f, 0.0f },
					{ 1.0f, 1.0f },
					img_color);
			}
		}

		void OnEntityAdded(const EntityWrapper& ent) override {
			if (ent.type != EntityType::Hero) return;

			renderData.insert({ ent->GetIndex(), RenderData() });
		};
		void OnEntityRemoved(const EntityWrapper& ent) override {
			renderData.erase(ent->GetIndex());
		};

		void OnFrame() override;

		M_SpeedIndicator() : IFrameListener() {
			IEntityListListener::Subscribe(this);

		}
	} SpeedIndicator;
}