#pragma once
#include "../../SDK/pch.h"

namespace Hacks {
	// Probably should also check for enemy presence in the process of casting
	class BadCastPrevention {
		const std::vector<const char*> pointAbilityNames = {
			"faceless_void_chronosphere",
			"enigma_black_hole"
		};
	public:
		bool AreEnemyUnitsInArea(const Vector& center, int radius);
		bool AreEnemyHeroesInArea(const Vector& center, int radius);

		// Checks whether the ability is cast at an area without enemy heroes/units
		bool IsBadCast(uint32_t abilityIdx, Vector* pos, CBaseEntity* caster);
	};
}
namespace Modules {
	inline Hacks::BadCastPrevention BadCastPrevention{};
}