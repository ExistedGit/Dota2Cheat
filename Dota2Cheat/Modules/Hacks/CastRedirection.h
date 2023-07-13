#pragma once
#include "../../pch.h"
#include "../../CheatSDK/include.h"

namespace Modules {
	//Redirects spell casts from illusions to the real hero
	//If the hero is not seen or is too far the spell cast is cancelled
	inline class M_CastRedirection {
		public:
			bool RedirectIfIllusionCast(uint32_t& targetIndex, CBaseEntity* issuer, uint32_t abilityIndex, bool& giveOrder);
	} CastRedirection{};
}