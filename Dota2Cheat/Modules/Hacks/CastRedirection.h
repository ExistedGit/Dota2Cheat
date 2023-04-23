#pragma once
#include "../../CheatSDK/Config.h"
#include "../../SDK/pch.h"
namespace Hacks {
	//Redirects spell casts from illusions to the real hero
	//If the hero is not seen or is too far the spell cast is cancelled
	class CastRedirection {
	public:
		bool RedirectIfIllusionCast(uint32_t& targetIndex, CBaseEntity* issuer, uint32_t abilityIndex, bool& giveOrder);
	};
}

namespace Modules {
	inline Hacks::CastRedirection CastRedirection{};
}