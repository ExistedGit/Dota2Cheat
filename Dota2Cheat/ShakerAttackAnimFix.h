#pragma once
#include "Wrappers.h"


namespace Hacks {
	// Changes Shaker's default attack animation to Enchant Totem's one
	// rofl feature from melonity :)
	class ShakerAttackAnimFix {
	private:
		BaseNpc* shaker = nullptr;
	public:
		void SubscribeEntity(BaseNpc* hero) {
			if (!strcmp(hero->GetUnitName(), "npc_dota_hero_earthshaker"))
				shaker = hero;
		}
		void Reset() {
			shaker = nullptr;
		}

		void ChangeAttackAnimIfNeeded(NetMessageHandle_t* msgHandle, google::protobuf::Message* msg) {
			if (msgHandle->messageID != 521 ||
				!shaker)
				return;

			auto animMsg = reinterpret_cast<CDOTAUserMsg_TE_UnitAnimation*>(msg);
			if (animMsg->activity() == 1503 &&
				NH2IDX(animMsg->entity()) == H2IDX(shaker->GetIdentity()->entHandle))
				animMsg->set_sequence_variant(3);
			
		}
	};
}
namespace Modules {
	inline Hacks::ShakerAttackAnimFix ShakerAttackAnimFix{};
}