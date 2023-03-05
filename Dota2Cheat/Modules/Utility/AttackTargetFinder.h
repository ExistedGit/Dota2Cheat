#pragma once
#include "../../SDK/Entities/CDOTABaseNPCHero.h"
#include "../../SDK/Globals/Context.h"
#include "../../SDK/Globals/Interfaces.h"

class AttackTargetFinder {
public:
	static CBaseEntity* GetAttackTarget(CDOTABaseNPC* attacker, int step = 25, float searchRadius = 25) {
		for (int i = 0; i <= attacker->GetAttackRange(); i += step) {
			for (int j = 0; j <= Interfaces::EntitySystem->GetHighestEntityIndex(); j++) {
				auto pos = attacker->GetForwardVector(i);
				auto ent = Interfaces::EntitySystem->GetEntity(j);
				if (ent &&
					ent->GetTeam() != attacker->GetTeam() &&
					IsWithinRadius(
						pos,
						ent->GetPos(),
						searchRadius)
					&& ctx.heroes.count((CDOTABaseNPC_Hero*)ent)
					)
					return ent;
			}
		}
		return nullptr;
	}
};