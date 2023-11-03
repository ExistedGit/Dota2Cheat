#pragma once
#include "../../pch.h"
#include "../../CheatSDK/include.h"
#include "../../Utils/Drawing.h"
#include "MultiThreadModule.h"
#include "../MListeners.h"

namespace Modules {
	// Shows the exact point of QoP's and Antimage's blinks
	inline class M_BlinkRevealer : public MultiThreadModule, public IFrameListener, public INetChanListener{
		struct BlinkData {
			Vector pos{ 0, 0,0 };
			bool qop;
			float fadeCounter, fadeTime;
		};
		std::map<CBaseEntity*, BlinkData> Blinks;

		float lastTime = 0;
	public:
		void Draw();
		void OnFrame() override;
		void OnReceivedMsg(NetMessageHandle_t* msgHandle, google::protobuf::Message* msg) override;
	} BlinkRevealer{};
}