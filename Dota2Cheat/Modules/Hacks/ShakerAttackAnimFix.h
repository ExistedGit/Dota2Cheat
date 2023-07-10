#pragma once
#include "../../SDK/pch.h"
#include "../../SDK/Protobufs/dota_usermessages.pb.h"

namespace Modules {
inline 
	// Changes Shaker's default attack animation to Enchant Totem's one
	// rofl feature from melonity :)
	class ShakerAttackAnimFix {
	private:
		CDOTABaseNPC* shaker = nullptr;
	public:
		void Reset() {
			shaker = nullptr;
		}

		void SubscribeEntity(CDOTABaseNPC* hero) {
			if (auto unitName = hero->GetUnitName()) {
				if (!strcmp(unitName, "npc_dota_hero_earthshaker"))
					shaker = hero;
				else
					Reset();
			}
		}

		void OnReceivedMsg(NetMessageHandle_t* msgHandle, google::protobuf::Message* msg) {
			if (msgHandle->messageID != 521 || !shaker)
				return;

			auto animMsg = reinterpret_cast<CDOTAUserMsg_TE_UnitAnimation*>(msg);
			if (animMsg->activity() == 1503 &&
				NH2IDX(animMsg->entity()) == shaker->GetIndex())
				animMsg->set_sequence_variant(3);

		}
	} ShakerAttackAnimFix;
}