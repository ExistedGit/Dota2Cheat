#pragma once
#include "../SDK/pch.h"
#include "../CheatSDK/Config.h"

namespace Hacks {
	// Absolutely ebin stubb :DDDD
	class M_AutoLastHit {
	public:
		int GetHealthAfterProjectiles(CDOTABaseNPC* creep, float time) {
			if (!GameSystems::ProjectileManager)
				return -1;

			int health = creep->GetHealth();
			for (auto& proj : GameSystems::ProjectileManager->GetTrackingProjectiles()) {
				if (!proj->IsAttack() || proj->GetTarget() != creep || !proj->GetSource())
					continue;

				auto dist = proj->GetPos().As<Vector2D>().DistTo(creep->GetPos().As<Vector2D>());
				auto timeDelta = time - GameSystems::GameRules->GetGameTime();
				if (dist / proj->GetMoveSpeed() < timeDelta)
					health -= proj->GetSource()->GetAttackDamageMin();
			}
			return health;
		}
		void WillLastHit() {

		}
	};
}

namespace Modules {
	inline Hacks::AutoLastHit AutoLastHit{};
}