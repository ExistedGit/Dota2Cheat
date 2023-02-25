#pragma once
#include "sdk.h"
#include <google/protobuf/message.h>
#include "SDK/CNetworkMessages.h"
#include "Wrappers.h"

class ProjectileTracker {

public:
	struct Projectile {
		BaseEntity* source, *target;
		int moveSpeed;
		uint32_t particleSystemHandle;
		bool dodgeable, isAttack;
		uint32_t launchTick, 
			handle;
	};

	void QueueIfProjectile(NetMessageHandle_t* msgHandle, google::protobuf::Message* msg) {
		if (msgHandle->messageID == 518) {
			auto projMsg = reinterpret_cast<CDOTAUserMsg_TE_Projectile*>(msg);
			
		}
	}
};