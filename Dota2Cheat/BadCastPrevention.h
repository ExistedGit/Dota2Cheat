#pragma once
#include "Wrappers.h"

namespace Hacks {
	// Probably should also check for enemy presence in the process of casting
	class BadCastPrevention {
	private:
		const std::vector<const char*> pointAbilityNames = {
			"faceless_void_chronosphere",
			"enigma_black_hole",
			"ember_spirit_sleight_of_fist"
		};
	public:
		bool AreHeroesInArea(const Vector2& center, int radius) {
			for (auto& hero : heroes) {
				if (
					hero->IsTargetable() &&
					hero->GetTeam() != assignedHero->GetTeam() &&
					IsWithinRadius(center, hero->GetPos2D(), radius)
					)
					return true;
			}
			return false;
		}

		// Checks if the ability can be handled by this module
		bool IsBadCast(uint32_t abilityIdx, Vector3* pos, BaseEntity* caster) {
			auto abilityName = Interfaces::EntitySystem->GetIdentity(abilityIdx)->GetName();
			auto ability = Interfaces::EntitySystem->GetEntity<BaseAbility>(abilityIdx);

			if (abilityName) {
				if (TestStringFilters(abilityName, pointAbilityNames))
					return !AreHeroesInArea(pos->AsVec2(), ability->GetAOERadius());
				else {
					if (strstr(abilityName, "magnataur_reverse_polarity"))
						// don't ask why RP's radius is stored as cast range.
						// I don't know
						return !AreHeroesInArea(caster->GetPos2D(), ability->GetCastRange());
					else if (strstr(abilityName, "earthshaker_echo_slam"))
						return !AreHeroesInArea(caster->GetPos2D(), ability->GetLevelSpecialValueFor("echo_slam_echo_search_range"));
					
				}
			}

			return false;
		}

	};
}
namespace Modules {
	Hacks::BadCastPrevention BadCastPrevention{};
}