#pragma once
#include "../../Config.h"
#include "../../SDK/include.h"
namespace Hacks {
	//Redirects spell casts from illusions to the real hero
	//If the hero is not seen or is too far the spell cast is cancelled
	class CastRedirection {
	public:
		bool RedirectIfIllusionCast(uint32_t& targetIndex, CBaseEntity* issuer, uint32_t abilityIndex, bool& giveOrder) {
			if (!Config::CastRedirection)
				return false;
			auto npc = Interfaces::EntitySystem->GetEntity<CDOTABaseNPC>(targetIndex);

			if (!ctx.heroes.count((CDOTABaseNPC_Hero*)npc) ||
				!reinterpret_cast<CDOTABaseNPC_Hero*>(npc)->IsIllusion())
				return false;

			auto illusionOwner = Interfaces::EntitySystem->GetEntity<CDOTAPlayerController>(
				H2IDX(npc->GetOwnerEntityHandle())
				)
				->GetAssignedHero();

			auto range = Interfaces::EntitySystem->GetEntity<CDOTABaseAbility>(abilityIndex)->GetEffectiveCastRange();
			if (!illusionOwner->IsTargetable() ||
				!IsWithinRadius(illusionOwner->GetPos(), issuer->GetPos(), range * 1.4)) {
				giveOrder = false;
				return false;
			}

			targetIndex = illusionOwner->GetIndex();

			return true;
		}
	};
}

namespace Modules {
	inline static Hacks::CastRedirection CastRedirection{};
}