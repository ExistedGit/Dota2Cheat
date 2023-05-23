#include "BadCastPrevention.h"
#include "../../CheatSDK/Config.h"

bool Hacks::BadCastPrevention::AreEnemyUnitsInArea(const Vector& center, int radius) {
	for (int i = 0; i <= Interfaces::EntitySystem->GetHighestEntityIndex(); i++) {
		auto ent = Interfaces::EntitySystem->GetEntity(i);
		if (ent
			&& !ent->IsSameTeam(ctx.localHero)
			&& IsWithinRadius(center, ent->GetPos(), radius)
			&& (ctx.heroes.contains((CDOTABaseNPC_Hero*)ent) || ctx.creeps.contains((CDOTABaseNPC*)ent))
			&& reinterpret_cast<CDOTABaseNPC*>(ent)->IsTargetable())
			return true;
	}
	return false;
}

bool Hacks::BadCastPrevention::AreEnemyHeroesInArea(const Vector& center, int radius) {
	for (auto& hero : ctx.heroes) {
		if (
			hero->IsTargetable()
			&& !hero->IsSameTeam(ctx.localHero)
			&& IsWithinRadius(center, hero->GetPos(), radius)
			)
			return true;
	}
	return false;
}

// Checks whether the ability is cast at an area without enemy heroes/units
bool Hacks::BadCastPrevention::IsBadCast(dotaunitorder_t orderType, UINT32 targetIdx, Vector* pos, UINT32 abilityIdx, CBaseEntity* issuer) {
	if (!Config::BadCastPrevention)
		return false;

	auto ability = Interfaces::EntitySystem->GetEntity<CDOTABaseAbility>(abilityIdx);

	if (!ability->GetIdentity()->GetName())
		return false;

	std::string abilityName = Interfaces::EntitySystem->GetIdentity(abilityIdx)->GetName();
	if (orderType == DOTA_UNIT_ORDER_CAST_TARGET &&
		EntCastBehaviors.contains(abilityName))
		return !EntCastBehaviors.at(abilityName)(ability, (CDOTABaseNPC*)issuer, Interfaces::EntitySystem->GetEntity<CDOTABaseNPC>(targetIdx));

	if (TestStringFilters(abilityName, pointAbilityNames))
		return !AreEnemyHeroesInArea(*pos, ability->GetAOERadius());
	switch (CityHash32(abilityName)) {

	case "magnataur_reverse_polarity"_city32:
		// don't ask why RP's radius is stored as cast range.
		// I don't know
		return !AreEnemyHeroesInArea(issuer->GetPos(), ability->GetCastRange());
	case "earthshaker_echo_slam"_city32:
		return !AreEnemyHeroesInArea(issuer->GetPos(), ability->GetLevelSpecialValueFor("echo_slam_echo_search_range"));
	case "ember_spirit_sleight_of_fist"_city32:
		// it's also frequently used on creeps
		return !AreEnemyUnitsInArea(*pos, ability->GetAOERadius());
	}

	return false;
}
