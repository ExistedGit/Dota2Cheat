#pragma once
#include "../../CheatSDK/Config.h"
#include <Dota2SDK.h>

namespace Modules {
	// Bypasses Blink Dagger's overshoot mechanic
	inline class M_PerfectBlink {
	public:
		bool AdjustIfBlink(Vector* position, uint32_t abilityIndex, CBaseEntity* issuer);
	}
	PerfectBlink{};
}