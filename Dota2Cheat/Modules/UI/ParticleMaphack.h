#pragma once
#include "../../CheatSDK/include.h"
#include "../../Utils/Drawing.h"
#include "MultiThreadModule.h"
#include "../MListeners.h"

namespace Modules {
	inline class M_ParticleMaphack : MultiThreadModule, public INetChanListener, public IFrameListener, public IEntityListListener {
		struct AppearanceData {
			Vector worldPos{ 0, 0,0 };
			ImVec2 mapPos{ 0, 0 };
			ImTextureID icon;
			float fadeCounter{}, fadeTime{};
		};
		// Particles which are created with attach type 2 and are then transformed with separate messages
		std::map<uint32_t, CDOTABaseNPC_Hero*> TransformQueue;
		std::set<uint32_t> UnidentifiedQueue;

		// Only shows the last one
		std::map<CDOTABaseNPC*, AppearanceData> Appearances;
		std::map<uint32_t, AppearanceData> UnidentifiedAppearances;

		float lastTime = 0;
		
		void OnEntityRemoved(const EntityWrapper& w) override {
			MTM_LOCK;

			Appearances.erase((CDOTABaseNPC*)w.ent);
		}

		void OnEntityAdded(const EntityWrapper& ent) override {}

		void DrawScreenAppearances();
		void DrawMapAppearances();
		void RegisterAppearance(CDOTABaseNPC* npc, const Vector& pos);
	public:
		void Draw();
		void OnFrame() override;
		void OnReceivedMsg(NetMessageHandle_t* msgHandle, google::protobuf::Message* msg) override;

		M_ParticleMaphack() : IFrameListener() {
			IEntityListListener::Subscribe(this);
		}
	} ParticleMaphack{};
}