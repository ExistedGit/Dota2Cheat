#pragma once
#include "../../pch.h"
#include "../../Utils/Drawing.h"
#include "../../CheatSDK/include.h"
#include "MultiThreadModule.h"
#include "../MListeners.h"
#include <list>

namespace Modules {
	inline class M_TPTracker : MultiThreadModule, public IFrameListener, public INetChanListener {
		struct TPData {
			ImVec2 pos;
			uint32_t msgIdx = 0;
		};

		struct TPLineData {
			CNPC* ent;
			ImU32 color;
			ImTextureID icon{};
			bool isFading = false;
			bool cancelled = false;
			float fadeDuration = 0.0f;
			float fadeCounter = 0;
			TPData start, end;
		};

		std::list<TPLineData> teleports;

		float lastTime = 0;
		void UpdateTeleport(uint32_t msgIndex, const Vector& pos) {
			for (auto& data : teleports) {
				if (data.start.msgIdx == msgIndex) {
					data.start.pos = WorldToMap(pos);
					break;
				}
				else if (data.end.msgIdx == msgIndex) {
					data.end.pos = WorldToMap(pos);
					break;
				}
			}
		}
	public:
		void Reset() {
			MTM_LOCK;
			teleports.clear();
		}

		// Mostly calculating fade duration
		void OnFrame() override;
		void Draw();
		void OnReceivedMsg(NetMessageHandle_t* msgHandle, google::protobuf::Message* msg) override;
	} TPTracker{};
}