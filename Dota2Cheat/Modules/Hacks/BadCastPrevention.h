#pragma once
#include "../../SDK/include.h"

namespace Hacks {
	// Probably should also check for enemy presence in the process of casting
	class BadCastPrevention {
	private:
		const std::vector<const char*> pointAbilityNames = {
			"faceless_void_chronosphere",
			"enigma_black_hole"
		};
	public:
		bool AreEnemyUnitsInArea(const Vector& center, int radius) {
			for (int i = 0; i <= Interfaces::EntitySystem->GetHighestEntityIndex(); i++) {
				auto ent = Interfaces::EntitySystem->GetEntity(i);
				if (ent
					&& ent->GetTeam() != ctx.assignedHero->GetTeam()
					&& IsWithinRadius(center, ent->GetPos(), radius)

					&& ent->SchemaBinding()->binaryName
					&& TestStringFilters(ent->SchemaBinding()->binaryName, { "BaseNPC","Unit_Hero" })

					&& reinterpret_cast<CDOTABaseNPC*>(ent)->IsTargetable())
					return true;
			}
			return false;
		}

		bool AreEnemyHeroesInArea(const Vector& center, int radius) {
			for (auto& hero : ctx.heroes) {
				if (
					hero->IsTargetable() &&
					hero->GetTeam() != ctx.assignedHero->GetTeam() &&

					IsWithinRadius(center, hero->GetPos(), radius)
					)
					return true;
			}
			return false;
		}

		// Checks whether the ability is cast at an area without enemy heroes/units
		bool IsBadCast(uint32_t abilityIdx, Vector* pos, CBaseEntity* caster) {
			auto ability = Interfaces::EntitySystem->GetEntity<CDOTABaseAbility>(abilityIdx);

			if (Interfaces::EntitySystem->GetIdentity(abilityIdx)->GetName()) {

				std::string_view abilityName = Interfaces::EntitySystem->GetIdentity(abilityIdx)->GetName();
				if (TestStringFilters(abilityName, pointAbilityNames))
					return !AreEnemyHeroesInArea(*pos, ability->GetAOERadius());
				else {
					if (abilityName == "magnataur_reverse_polarity")
						// don't ask why RP's radius is stored as cast range.
						// I don't know
						return !AreEnemyHeroesInArea(caster->GetPos(), ability->GetCastRange());
					else if (abilityName == "earthshaker_echo_slam")
						return !AreEnemyHeroesInArea(caster->GetPos(), ability->GetLevelSpecialValueFor("echo_slam_echo_search_range"));
					else if (abilityName == "ember_spirit_sleight_of_fist") // it's also frequently used on creeps
						return !AreEnemyUnitsInArea(*pos, ability->GetAOERadius());
					// can be expanded indefinitely
				}
			}

			return false;
		}

	};
}
namespace Modules {
	inline static Hacks::BadCastPrevention BadCastPrevention{};
}