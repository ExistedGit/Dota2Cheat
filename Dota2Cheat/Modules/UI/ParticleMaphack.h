#pragma once
#include "../../SDK/pch.h"
#include "../../CheatSDK/include.h"
#include "../../Utils/Drawing.h"
#include "MultiThreadModule.h"

namespace Hacks {
	class ParticleMaphack : public MultiThreadModule {
		struct ParticleData {
			Vector pos{ 0, 0,0 };
			ImTextureID icon;
			float fadeCounter{}, fadeTime{};
		};
		struct ParticleEntData : public ParticleData {
			CDOTABaseNPC* ent{};
		};

		// Particles which are created with attach type 2 and then are transformed with separate messages
		std::map<int, CDOTABaseNPC_Hero*> TransformQueue;

		// Only shows the last one
		std::map<CDOTABaseNPC*, ParticleData> MapAppearances;
		std::vector<ParticleEntData> AllAppearances;

		float lastTime = 0;
		void DrawAllAppearances();
		void DrawMapAppearances();
		void RegisterAppearance(CDOTABaseNPC* npc, const Vector& pos) {
			ImTextureID icon = 0;
			GetHeroIcon(npc, icon);

			auto& mapData = MapAppearances[npc];
			mapData.pos = pos;
			mapData.fadeCounter = mapData.fadeTime = Config::ParticleMapHack::FadeDuration;
			mapData.icon = icon;

			auto data = ParticleEntData{
				.ent = npc
			};

			data.pos = pos;
			data.icon = icon;
			data.fadeCounter = data.fadeTime = Config::ParticleMapHack::FadeDuration;
			AllAppearances.push_back(data);
		}
	public:
		void Draw();
		void FrameBasedLogic();
		void ProcessParticleMsg(NetMessageHandle_t* msgHandle, google::protobuf::Message* msg);
		void GetHeroIcon(CDOTABaseNPC* npc, ImTextureID& icon);
	};
}
namespace Modules {
	inline Hacks::ParticleMaphack ParticleMaphack{};
}