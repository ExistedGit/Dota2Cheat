#pragma once
#include "../../SDK/pch.h"

namespace Hacks {

	class PerfectBlink {
	public:
		bool AdjustIfBlink(Vector* position, uint32_t abilityIndex, CBaseEntity* issuer);
	};
}
namespace Modules {
	inline Hacks::PerfectBlink PerfectBlink{};
}