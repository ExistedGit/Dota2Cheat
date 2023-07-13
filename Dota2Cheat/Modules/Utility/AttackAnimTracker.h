#pragma once
#include "../../pch.h"
#include "../../CheatSDK/include.h"
#include "AttackTargetFinder.h"

namespace Utility {
	class AttackAnimTracker {
		struct AttackAnimData {
			float startTime;
			float castPoint;
		};
		std::map<CDOTABaseNPC*, AttackAnimData> animations;
	public:
		bool WillUnitAttack(CDOTABaseNPC* unit, CDOTABaseNPC* target, float timeDelta);
		void ProcessAttackAnimMessage(NetMessageHandle_t* msgHandle, google::protobuf::Message* msg);
	};
}

namespace Modules {
	inline Utility::AttackAnimTracker AttackAnimTracker{};
}