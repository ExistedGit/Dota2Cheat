#pragma once
#include "../../SDK/pch.h"
#include "../../SDK/Protobufs/dota_usermessages.pb.h"

namespace Hacks {
	// Changes Shaker's default attack animation to Enchant Totem's one
	// rofl feature from melonity :)
	class ShakerAttackAnimFix {
	private:
		CDOTABaseNPC* shaker = nullptr;
	public:
		void SubscribeEntity(CDOTABaseNPC* hero);
		void Reset();

		void ChangeAttackAnimIfNeeded(NetMessageHandle_t* msgHandle, google::protobuf::Message* msg);
	};
}
namespace Modules {
	inline Hacks::ShakerAttackAnimFix ShakerAttackAnimFix{};
}