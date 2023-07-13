#pragma once
#include "../../pch.h"
namespace Utility {
	class AttackTargetFinder {
	public:
		CBaseEntity* GetAttackTarget(CDOTABaseNPC* attacker, int step = 25, float searchRadius = 12.5);
	};
}
namespace Modules{
	inline Utility::AttackTargetFinder AttackTargetFinder{};
}