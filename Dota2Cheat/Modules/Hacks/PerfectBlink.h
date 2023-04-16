#pragma once
#include "../../SDK/pch.h"
#include "../../Config.h"

namespace Hacks {

	class PerfectBlink {
	public:
		void AdjustIfBlink(Vector* position, uint32_t abilityIndex, CBaseEntity* issuer);
	};
}
namespace Modules {
	inline Hacks::PerfectBlink PerfectBlink{};
}