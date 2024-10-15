#pragma once
#include "../pch.h"
#include "../Utils/Drawing.h"
#include "../CheatSDK/include.h"
#include "../MListeners.h"

namespace Modules {
	inline class M_LastHitMarker : public IEntityListListener, public IFrameListener {
	public:
		class RenderData {
			bool isSiege = false;
		public:
			bool IsSiege() const {
				return isSiege;
			}
			bool drawable = false;

			ImVec2 pos;

			bool allied;
			RenderData(bool isSiege) : isSiege(isSiege) {}
			RenderData() {}
		};

		std::map<entidx_t, RenderData> renderData;

		void Draw();

		void OnEntityAdded(const EntityWrapper& ent) override {
			if (ent.creepType == CreepType::NotCreep) return;

			renderData[ent->GetIndex()] = RenderData(ent.creepType == CreepType::Siege);
		};

		void OnEntityRemoved(const EntityWrapper& ent) override {
			renderData.erase(ent->GetIndex());
		};

		void OnFrame() override;

		M_LastHitMarker() : IFrameListener() { 
			IEntityListListener::Subscribe(this);
		}
	} LastHitMarker{};
}