#pragma once
#include "../../pch.h"
#include "../../CheatSDK/Config.h"

namespace Modules {
	// Bypasses Blink Dagger's overshoot mechanic
	inline class M_PerfectBlink {
	public:
		bool AdjustIfBlink(Vector* position, uint32_t abilityIndex, CBaseEntity* issuer);
	}
	PerfectBlink{};
}