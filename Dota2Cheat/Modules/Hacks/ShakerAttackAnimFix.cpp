#include "ShakerAttackAnimFix.h"

void Hacks::ShakerAttackAnimFix::SubscribeEntity(CDOTABaseNPC* hero) {
	if (auto unitName = hero->GetUnitName()) {
		if (!strcmp(unitName, "npc_dota_hero_earthshaker"))
			shaker = hero;
		else
			Reset();
	}
}

void Hacks::ShakerAttackAnimFix::Reset() {
	shaker = nullptr;
}

void Hacks::ShakerAttackAnimFix::ChangeAttackAnimIfNeeded(NetMessageHandle_t* msgHandle, google::protobuf::Message* msg) {
	if (msgHandle->messageID != 521 ||
		!shaker)
		return;

	auto animMsg = reinterpret_cast<CDOTAUserMsg_TE_UnitAnimation*>(msg);
	if (animMsg->activity() == 1503 &&
		NH2IDX(animMsg->entity()) == shaker->GetIndex())
		animMsg->set_sequence_variant(3);

}
