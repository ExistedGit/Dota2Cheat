#pragma once
#include "../../SDK/pch.h"
#include "../../CheatSDK/Config.h"

namespace Modules {
	inline class M_PerfectBlink {
	public:
		bool AdjustIfBlink(Vector* position, uint32_t abilityIndex, CBaseEntity* issuer);
	}
	PerfectBlink{};
}