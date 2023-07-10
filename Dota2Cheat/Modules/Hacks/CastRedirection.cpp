#include "CastRedirection.h"

bool Modules::CastRedirection::RedirectIfIllusionCast(uint32_t& targetIndex, CBaseEntity* issuer, uint32_t abilityIndex, bool& giveOrder) {
	if (!Config::CastRedirection)
		return false;
	auto npc = Interfaces::EntitySystem->GetEntity<CDOTABaseNPC>(targetIndex);

	if (!ctx.heroes.count((CDOTABaseNPC_Hero*)npc) ||
		!reinterpret_cast<CDOTABaseNPC_Hero*>(npc)->IsIllusion())
		return false;

	CDOTABaseNPC_Hero* illusionOwner = npc->GetOwnerEntityHandle().Entity()->GetAssignedHeroHandle();

	auto range = Interfaces::EntitySystem->GetEntity<CDOTABaseAbility>(abilityIndex)->GetEffectiveCastRange();
	if (!illusionOwner->IsTargetable() ||
		!IsWithinRadius(illusionOwner->GetPos(), issuer->GetPos(), range * 1.4)) {
		giveOrder = false;
		return false;
	}

	targetIndex = illusionOwner->GetIndex();

	return true;
}
