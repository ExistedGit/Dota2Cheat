#pragma once
#include "Config.h"
#include "Interfaces.h"
#include "Wrappers.h"
namespace Hacks {
	//Redirects spell casts from illusions to the real hero
	class CastRedirection {
	public:
		bool RedirectIfIllusionCast(uint32_t& targetIndex, BaseEntity* issuer, uint32_t abilityIndex) {
			if (!Config::CastRedirection)
				return false;
			auto npc = Interfaces::EntitySystem->GetEntity<BaseNpc>(targetIndex);

			if (!ctx.heroes.count((BaseNpcHero*)npc) ||
				!reinterpret_cast<BaseNpcHero*>(npc)->IsIllusion())
				return false;

			auto illusionOwner = Interfaces::EntitySystem->GetEntity<DotaPlayer>(
				H2IDX(npc->GetOwnerEntityHandle())
				)
				->GetAssignedHero();

			auto range = Interfaces::EntitySystem->GetEntity<BaseAbility>(abilityIndex)->GetEffectiveCastRange();
			if (!illusionOwner->IsTargetable() ||
				!IsWithinRadius(illusionOwner->GetPos2D(), issuer->GetPos2D(), range * 1.25))
				return false;

			targetIndex =
				H2IDX(
					illusionOwner
					->GetIdentity()
					->entHandle
				);

			return true;
		}
	};
}

namespace Modules {
	Hacks::CastRedirection CastRedirection{};
}