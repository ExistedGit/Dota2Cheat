#pragma once

namespace Utility {
	class AttackTargetFinder {
	public:
		CBaseEntity* GetAttackTarget(CDOTABaseNPC* attacker, int step = 25, float searchRadius = 25);
	};
}
namespace Modules{
	inline Utility::AttackTargetFinder AttackTargetFinder{};
}