#include "CastRedirection.h"

bool Modules::M_CastRedirection::RedirectIfIllusionCast(uint32_t& targetIndex, CBaseEntity* issuer, uint32_t abilityIndex, bool& giveOrder) {
	if (!Config::CastRedirection)
		return false;
	auto npc = Interfaces::EntitySystem->GetEntity<CDOTABaseNPC_Hero>(targetIndex);

	if (!EntityList.IsHero(npc) ||
		!npc->IsIllusion())
		return false;

	CDOTABaseNPC_Hero* illusionOwner = npc->GetOwnerEntityHandle().Entity()->GetAssignedHeroHandle();

	auto range = Interfaces::EntitySystem->GetEntity<CDOTABaseAbility>(abilityIndex)->GetEffectiveCastRange();
	if (!illusionOwner->IsTargetable()) {
		ShowHUDError("Cannot redirect cast: target missing", "General.InvalidTarget_Invulnerable");
		giveOrder = false;
	}
	else if (!IsWithinRadius(illusionOwner->GetPos(), issuer->GetPos(), range * 1.4)) {
		ShowHUDError("Cannot redirect cast: target too far", "General.InvalidTarget_Invulnerable");
		giveOrder = false;
	}

	if (!giveOrder)
		return false;

	targetIndex = illusionOwner->GetIndex();

	return true;
}
