#pragma once
#include "sdk.h"
#include "SDK/CNetworkMessages.h"
#include "Wrappers.h"
#include "Globals.h"

// STUB, need to produce highly a sophisticated mechanism to track non-linear stuff
class ProjectileTracker {

public:
	struct Projectile {
		BaseEntity* source, * target;
		int moveSpeed;
		int64_t particleSystemHandle;
		bool dodgeable, isAttack;
		int32_t launchTick, handle;
	};

	std::set<Projectile> projectiles;


	//float lastGameTime = 0;
	//void FrameBasedLogic() {
	//	float delta = lastGameTime - Globals::GameRules->GetGameTime();
	//	lastGameTime = Globals::GameRules->GetGameTime();
	//	for (auto& [handle, proj] : linearProjectiles) {
	//		
	//	}
	//}
	
	void QueueIfProjectile(NetMessageHandle_t* msgHandle, google::protobuf::Message* msg) {
		if (msgHandle->messageID == 518) {
			auto projMsg = reinterpret_cast<CDOTAUserMsg_TE_Projectile*>(msg);
			projectiles.insert(Projectile{
					.source = Interfaces::EntitySystem->GetEntity(H2IDX(projMsg->source())),
					.target = Interfaces::EntitySystem->GetEntity(H2IDX(projMsg->target())),
					.moveSpeed = projMsg->move_speed(),
					.particleSystemHandle = projMsg->particle_system_handle(),
					.dodgeable = projMsg->dodgeable(),
					.isAttack = projMsg->is_attack(),
					.launchTick = projMsg->launch_tick(),
					.handle = projMsg->handle()
				});
		}
	}
};

//namespace Modules {
//	ProjectileTracker ProjectileTracker{};
//}