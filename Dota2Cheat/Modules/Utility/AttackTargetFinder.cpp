#include "AttackTargetFinder.h"
#include "../../CheatSDK/Data/EntityList.h"

CBaseEntity* Utility::AttackTargetFinder::GetAttackTarget(CDOTABaseNPC* attacker, int step, float searchRadius) {
	for (int i = 0; i <= attacker->GetAttackRange(); i += step) {
		auto pos = attacker->GetForwardVector(i);
		return EntityList.Find([this, pos, attacker, searchRadius](const auto& wrap) {
			return !wrap->IsSameTeam(attacker) &&
			IsWithinRadius(
				pos,
				wrap->GetPos(),
				searchRadius);
			});

	}
	return nullptr;
}
