#include "AttackTargetFinder.h"

CBaseEntity* Utility::AttackTargetFinder::GetAttackTarget(CDOTABaseNPC* attacker, int step, float searchRadius) {
	for (int i = 0; i <= attacker->GetAttackRange(); i += step) {
		for (auto& ent : ctx.heroes) {
			auto pos = attacker->GetForwardVector(i);
			if (ent &&
				!ent->IsSameTeam(attacker) &&
				IsWithinRadius(
					pos,
					ent->GetPos(),
					searchRadius))
				return ent;
		}
	}
	return nullptr;
}
