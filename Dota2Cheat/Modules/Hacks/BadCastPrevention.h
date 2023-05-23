#pragma once
#include "../../SDK/pch.h"
#include "../../CheatSDK/include.h"
#include <functional>
#include "cityhash/city.h"
#include "consthash/cityhash32.hxx"

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
		const std::map<std::string, std::function<bool(CDOTABaseAbility* spell, CDOTABaseNPC* caster, CDOTABaseNPC* target)>> EntCastBehaviors = {
			{
				"grimstroke_soul_chain", [this](CDOTABaseAbility* spell, CDOTABaseNPC* caster, CDOTABaseNPC* target) -> bool {
					const static auto latchRadius = spell->GetLevelSpecialValueFor("chain_latch_radius");
					for (auto& hero : ctx.heroes) {
						if (hero != target &&
							hero->IsSameTeam(target)
							&& IsWithinRadius(hero->GetPos(), target->GetPos(), latchRadius))
							return true;
								}
					return false;
				}
			}
		};

		// Checks whether the ability is cast at an area without enemy heroes/units
		bool IsBadCast(dotaunitorder_t orderType, UINT32 targetIndex, Vector* position, UINT32 abilityIndex, CBaseEntity* issuer);
	};
}
namespace Modules {
	inline Hacks::BadCastPrevention BadCastPrevention{};
}