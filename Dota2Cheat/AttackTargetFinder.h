#pragma once
#include "Wrappers.h"
#include "Interfaces.h"

class AttackTargetFinder {
public:
	static BaseEntity* GetAttackTarget(BaseNpc* attacker, int step = 25, float searchRadius = 25) {
		for (int i = 0; i <= attacker->GetAttackRange(); i += step) {
			for (int j = 0; j <= Interfaces::EntitySystem->GetHighestEntityIndex(); j++) {
				auto pos = attacker->GetForwardVector(i).AsVec2();
				auto ent = Interfaces::EntitySystem->GetEntity(j);
				if (ent != nullptr &&
					ent->GetTeam() != attacker->GetTeam() &&
					IsWithinRadius(
						pos,
						ent->GetPos2D(),
						searchRadius)
					)
					return ent;
			}
		}
		return nullptr;
	}
};