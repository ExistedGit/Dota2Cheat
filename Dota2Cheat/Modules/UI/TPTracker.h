#pragma once
#include "../../SDK/pch.h"
#include "../../Utils/Drawing.h"
#include "../../CheatSDK/include.h"

namespace Hacks {
	class TPTracker {
		struct TPData {
			Vector pos{ 0,0,0 };
			uint32_t msgIdx = 0;
		};

		struct TPLineData {
			ImU32 color;
			bool isFading = false;
			bool cancelled = false;
			float fadeDuration = 0.0f;
			float fadeCounter = 0;
			TPData start, end;
		};

		qwemap<CBaseEntity*, TPLineData> teleports;
		std::map<CBaseEntity*, ImTextureID> heroIcons;

		float lastTime = 0;
	public:
		void Reset() {
			teleports.clear();
		}
		// Mostly calculating fade duration
		void FrameBasedLogic();
		void CacheHeroIcons();
		void DrawMapTeleports();
		void ProcessParticleMsg(NetMessageHandle_t* msgHandle, google::protobuf::Message* msg);;
	};
}
namespace Modules {
	inline Hacks::TPTracker TPTracker{};
}