#pragma once
#include "Wrappers.h"


namespace Hacks {
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
			if (!shaker)
				return;
			if (msgHandle->messageID == 521) {
				auto animMsg = reinterpret_cast<CDOTAUserMsg_TE_UnitAnimation*>(msg);
				if ((animMsg->entity() & 0x3fff) == H2IDX(shaker->GetIdentity()->entHandle)) {
					animMsg->set_sequence_variant(3);
				};
			}
		}
	};
}
namespace Modules {
	inline Hacks::ShakerAttackAnimFix ShakerAttackAnimFix{};
}