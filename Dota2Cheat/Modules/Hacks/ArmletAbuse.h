#pragma once
#include "../../SDK/pch.h"

namespace Hacks {
	// STUB
	class ArmletAbuse {
	public:
		void SwitchArmletIfNeeded() {
			//if (!GameSystems::ProjectileManager)
			//	return;

			//for (auto& proj : GameSystems::ProjectileManager->m_pTrackingProjectiles) {
			//	if (!proj || !proj->IsAttack() ||
			//		!HeroData[ctx.localHero].Items["armlet"])
			//		continue;

			//	auto target = proj->GetTarget();
			//	auto source = proj->GetSource();
			//	if (target != ctx.localHero || !source)
			//		continue;

			//	auto armlet = HeroData[ctx.localHero].Items["armlet"];
			//	auto attackDamage = source->GetAttackDamageMin();
			//	if (attackDamage >= target->GetHealth()) {

			//	};

			//}
		}
	};

}
namespace Modules {
	inline Hacks::ArmletAbuse ArmletAbuse{};
}