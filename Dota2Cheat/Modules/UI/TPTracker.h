#pragma once
#include "../../pch.h"
#include "../../Utils/Drawing.h"
#include "../../CheatSDK/include.h"
#include "MultiThreadModule.h"
#include "../MListeners.h"

namespace Modules {
	inline class M_TPTracker : MultiThreadModule, public IFrameListener, public INetChanListener {
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
		void OnFrame() override;
		void DrawMapTeleports();
		void OnReceivedMsg(NetMessageHandle_t* msgHandle, google::protobuf::Message* msg) override;
	} TPTracker{};
}