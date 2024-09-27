#include "BadCastPrevention.h"
#include "../../CheatSDK/Config.h"

bool Modules::BCP::M_BadCastPrevention::AreEnemyUnitsInArea(const Vector& center, int radius) {
	const auto condition = [this, center, radius](const auto& ent) ->bool {
		return !ent->IsSameTeam(ctx.localHero)
			&& IsWithinRadius(center, ent->GetPos(), radius)
			&& ((CDOTABaseNPC*)ent)->IsTargetable();
	};
	return EntityList.ContainsTypes(condition, EntityType::Hero, EntityType::Creep);
}

bool Modules::BCP::M_BadCastPrevention::AreEnemyHeroesInArea(const Vector& center, int radius) {
	const auto condition = [this, center, radius](const auto& ent) ->bool {
		return !ent->IsSameTeam(ctx.localHero)
			&& IsWithinRadius(center, ent->GetPos(), radius)
			&& ((CDOTABaseNPC*)ent)->IsTargetable();
	};
	return EntityList.ContainsTypes(condition, EntityType::Hero);
}

// Checks whether the ability is cast at an area without enemy heroes/units
bool Modules::BCP::M_BadCastPrevention::IsBadCast(dotaunitorder_t orderType, UINT32 targetIdx, Vector* pos, UINT32 abilityIdx, CBaseEntity* issuer) {
	if (!Config::BadCastPrevention)
		return false;

	auto ability = CEntSys::Get()->GetEntity<CDOTABaseAbility>(abilityIdx);

	if (!ability->GetIdentity()->GetName())
		return false;

	std::string abilityName = ability->GetIdentity()->GetName();
	if (orderType == DOTA_UNIT_ORDER_CAST_TARGET &&
		castHandlers.contains(abilityName))
		return !castHandlers.at(abilityName)(ability, (CDOTABaseNPC*)issuer, CEntSys::Get()->GetEntity<CDOTABaseNPC>(targetIdx));

	if(pointAbilityNames.contains(abilityName))
		return !AreEnemyHeroesInArea(*pos, ability->GetAOERadius());

	// don't ask why RP's radius is stored as cast range.
	// I don't know
	if (abilityName == "magnataur_reverse_polarity" ||
		abilityName == "tinker_heat_seeking_missile")
		return !AreEnemyHeroesInArea(issuer->GetPos(), ability->GetCastRange());
	else if (abilityName == "earthshaker_echo_slam")
		return !AreEnemyHeroesInArea(issuer->GetPos(), ability->GetLevelSpecialValueFor("echo_slam_echo_search_range"));
	else if (abilityName == "ember_spirit_sleight_of_fist")
		// it's also frequently used on creeps
		return !AreEnemyUnitsInArea(*pos, ability->GetAOERadius());

	return false;
}
