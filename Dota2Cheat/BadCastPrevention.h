#pragma once
#include "Wrappers.h"

namespace Hacks {
	// Probably should also check for enemy presence in the process of casting
	class BadCastPrevention {
	private:
		const std::vector<const char*> pointAbilityNames = {
			"faceless_void_chronosphere",
			"enigma_black_hole"
		};
	public:
		bool AreEnemyUnitsInArea(const Vector2& center, int radius) {
			for (int i = 0; i <= Interfaces::EntitySystem->GetHighestEntityIndex(); i++) {
				auto ent = Interfaces::EntitySystem->GetEntity(i);
				if (ent 
					&& ent->GetTeam() != ctx.assignedHero->GetTeam()

					&& ent->SchemaBinding()->binaryName
					&& TestStringFilters(ent->SchemaBinding()->binaryName, { "BaseNPC","Unit_Hero" })

					&& reinterpret_cast<BaseNpc*>(ent)->IsTargetable()
					&& IsWithinRadius(center, ent->GetPos2D(), radius))

					return true;
			}
			return false;
		}

		bool AreEnemyHeroesInArea(const Vector2& center, int radius) {
			for (auto& hero : ctx.heroes) {
				if (
					hero->IsTargetable() &&
					hero->GetTeam() != ctx.assignedHero->GetTeam() &&
					IsWithinRadius(center, hero->GetPos2D(), radius)
					)
					return true;
			}
			return false;
		}

		// Checks whether the ability is cast at an area without enemy heroes/units
		bool IsBadCast(uint32_t abilityIdx, Vector3* pos, BaseEntity* caster) {
			auto abilityName = Interfaces::EntitySystem->GetIdentity(abilityIdx)->GetName();
			auto ability = Interfaces::EntitySystem->GetEntity<BaseAbility>(abilityIdx);

			if (abilityName) {
				if (TestStringFilters(abilityName, pointAbilityNames))
					return !AreEnemyHeroesInArea(pos->AsVec2(), ability->GetAOERadius());
				else {
					if (strstr(abilityName, "magnataur_reverse_polarity"))
						// don't ask why RP's radius is stored as cast range.
						// I don't know
						return !AreEnemyHeroesInArea(caster->GetPos2D(), ability->GetCastRange());
					else if (strstr(abilityName, "earthshaker_echo_slam"))
						return !AreEnemyHeroesInArea(caster->GetPos2D(), ability->GetLevelSpecialValueFor("echo_slam_echo_search_range"));
					else if (strstr(abilityName, "ember_spirit_sleight_of_fist")) // it's also frequently used on creeps
						return !AreEnemyUnitsInArea(pos->AsVec2(), ability->GetAOERadius());
					// can be expanded indefinitely
				}
			}

			return false;
		}

	};
}
namespace Modules {
	Hacks::BadCastPrevention BadCastPrevention{};
}