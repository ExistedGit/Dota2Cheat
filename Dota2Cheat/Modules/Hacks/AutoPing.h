#pragma once
#include "../../SDK/Protobufs/dota_clientmessages.pb.h"
#include "../../Hooks/NetChannel.h"
#include "../MListeners.h"

namespace Config {
	inline bool AutoPingEnabled = false;
	inline CDOTABaseNPC_Hero* AutoPingTarget = 0;
	inline float AutoPingDelay = 1;
}

namespace Modules {
	inline class M_AutoPing  : public IFrameListener {
		float lastGameTime = 0;
	public:
		void OnFrame() override;
	} AutoPing;
}