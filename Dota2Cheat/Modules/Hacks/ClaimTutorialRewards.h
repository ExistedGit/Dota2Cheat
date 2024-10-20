#pragma once
#include "../../CheatSDK/include.h"
#include <dota_gcmessages_client.pb.h>

namespace Modules {
	// Logic generously contributed by [DATA EXPUNGED]
	inline class M_ClaimTutorialRewards {
		void ClaimEventAction(uint32_t event_id, uint32_t action_id, uint32_t quantity = 1);
		// event_id to action_id
		using ClaimEventData = std::pair<uint32_t, uint32_t>;
	public:
		void ExecuteGCRequests();
	} ClaimTutorialRewards;
}