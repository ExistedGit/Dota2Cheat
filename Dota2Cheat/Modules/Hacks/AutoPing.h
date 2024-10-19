#pragma once
#include <dota_clientmessages.pb.h>
#include "../MListeners.h"

//namespace Config {
//	inline bool AutoPingEnabled = false;
//	inline CHero* AutoPingTarget = 0;
//	inline float AutoPingDelay = 1;
//}

namespace Modules {
	inline class M_AutoPing  : public IFrameListener {
		float lastGameTime = 0;
	public:
		void OnFrame() override;
	} AutoPing;
}