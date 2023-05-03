#pragma once
#include "../../SDK/pch.h"
#include "../../Utils/Drawing.h"
#include "../../CheatSDK/include.h"
#include "MultiThreadModule.h"

namespace Hacks {
	class TPTracker : public MultiThreadModule {
		struct TPData {
			Vector pos{ 0,0,0 };
			uint32_t msgIdx = 0;
		};

		struct TPLineData {
			ImU32 color;
			ImTextureID icon{};
			bool isFading = false;
			bool cancelled = false;
			float fadeDuration = 0.0f;
			float fadeCounter = 0;
			TPData start, end;
		};

		std::map<CBaseEntity*, TPLineData> teleports;

		float lastTime = 0;
	public:
		void Reset() {
			MTM_LOCK;
			teleports.clear();
		}
		// Mostly calculating fade duration
		void FrameBasedLogic();
		void DrawMapTeleports();
		void ProcessParticleMsg(NetMessageHandle_t* msgHandle, google::protobuf::Message* msg);;
	};
}
namespace Modules {
	inline Hacks::TPTracker TPTracker{};
}