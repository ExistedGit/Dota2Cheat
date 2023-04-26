#pragma once
#include "../../SDK/pch.h"

namespace Hacks {
	// STUB
	class ArmletAbuse {
	public:
		void SwitchArmletIfNeeded() {
			for (auto& proj : GameSystems::ProjectileManager->m_pTrackingProjectiles) {
				if (!proj || !proj->IsAttack() ||
					!ctx.ImportantItems["armlet"])
					continue;

				auto target = proj->GetTarget();
				auto source = proj->GetSource();
				if (target != ctx.assignedHero || !source)
					continue;

				auto armlet = ctx.ImportantItems["armlet"];
				auto attackDamage = source->GetAttackDamageMin();
				if (attackDamage >= target->GetHealth()) {

				};

			}
		}
	};

}
namespace Modules {
	inline Hacks::ArmletAbuse ArmletAbuse{};
}