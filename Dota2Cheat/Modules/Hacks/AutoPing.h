#pragma once
#include "../../SDK/Protobufs/dota_clientmessages.pb.h"
#include "../../Hooks/NetChannel.h"

namespace Config {
	inline bool AutoPingEnabled = false;
	inline CDOTABaseNPC_Hero* AutoPingTarget = 0;
	inline float AutoPingDelay = 1;
}

namespace Hacks {
	class AutoPing {
		float lastGameTime = 0;
	public:
		void FrameBasedLogic();
	};
}
namespace Modules {
	inline Hacks::AutoPing AutoPing;
}