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

		// Only shows the last one
		std::map<CDOTABaseNPC*, ParticleData> MapAppearances;
		std::vector<ParticleEntData> AllAppearances;

		float lastTime = 0;
		void DrawAllAppearances();
		void DrawMapAppearances();
	public:
		void Draw();
		void FrameBasedLogic();
		void ProcessParticleMsg(NetMessageHandle_t* msgHandle, google::protobuf::Message* msg);
	};
}
namespace Modules {
	inline Hacks::ParticleMaphack ParticleMaphack{};
}