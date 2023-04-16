#include "AttackAnimTracker.h"

bool Utility::AttackAnimTracker::WillUnitAttack(CDOTABaseNPC* unit, CDOTABaseNPC* target, float timeDelta) {
	if (!animations.count(unit))
		return false;
	auto anim = animations[unit];
	if (anim.startTime + anim.castPoint <= GameSystems::GameRules->GetGameTime() + timeDelta &&
		target == Modules::AttackTargetFinder.GetAttackTarget(unit))
		return true;

	return false;
}

void Utility::AttackAnimTracker::ProcessAttackAnimMessage(NetMessageHandle_t* msgHandle, google::protobuf::Message* msg) {
	if (msgHandle->messageID == 521)
	{
		auto animMsg = (CDOTAUserMsg_TE_UnitAnimation*)msg;

		if (animMsg->activity() != 1503)
			return;
		auto npc = Interfaces::EntitySystem->GetEntity<CDOTABaseNPC>(NH2IDX(animMsg->entity()));

		animations[npc] = {
			.startTime = GameSystems::GameRules->GetGameTime(),
			.castPoint = animMsg->castpoint() * animMsg->playbackrate(),
		};
	}
	else if (msgHandle->messageID == 522) {
		auto animMsg = (CDOTAUserMsg_TE_UnitAnimationEnd*)msg;
		auto npc = Interfaces::EntitySystem->GetEntity<CDOTABaseNPC>(NH2IDX(animMsg->entity()));
		animations.erase(npc);
	}

}
