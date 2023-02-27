#pragma once
#include "LinearProjectileWarner.h"
#include <map>
#include "Globals.h"
#include "Wrappers.h"

// particles\ui_mouseactions\range_finder_tower_aoe.vpcf
// Point 0: nothing
// Point 2: coord of the second end of the dashed line
// Point 3: X is boolean, controls some point in the center
// Point 4: color of that point
// Point 6: X is boolean, controls visibility of the particle
// Point 7: coord of the marker and the first end of the dashed line


// particles/ui_mouseactions/range_finder_tower_line.vpcf
// is the owner of control points 2, 6, 7

namespace Hacks {

	// Shows linear projectile trajectories with a red line
	// Also shows potential trajectories for aiming heroes like Hoodwink
	class LinearProjectileWarner {
	public:

		struct LinearProjectile {
			BaseEntity* source{};

			int32_t handle{};
			float distance{};
			Vector2 velocity{ 0, 0 };
			Vector2 acceleration{ 0, 0 };
			Vector3 origin{};
		};

		std::map<int32_t, CDOTAParticleManager::ParticleWrapper> TrackedProjectiles{};
		std::map<int32_t, LinearProjectile> linearProjectiles{};

		std::map<std::string_view, int> AbilityTrajectories{
			{"modifier_hoodwink_sharpshooter_windup", 3125}
		};

		struct EntTrajectoryInfo {
			int offset;
			CDOTAParticleManager::ParticleWrapper particleWrap;
		};

		std::map<DotaModifier*, EntTrajectoryInfo> EntityTrajectories{};

		// Draws a dashed red line from begin to end
		// Returns the wrapper for the created particle
		auto DrawTrajectory(Vector3 begin, Vector3 end) {
			auto pw = Globals::ParticleManager->CreateParticle(
				"particles/ui_mouseactions/range_finder_tower_line.vpcf",
				PATTACH_WORLDORIGIN,
				ctx.assignedHero
			);
			Vector3 boolCp{ 1, 0, 0 };
			pw.particle
				->SetControlPoint(2, &begin)
				->SetControlPoint(6, &boolCp)
				->SetControlPoint(7, &end);
			return pw;
		}


		void DrawIfTrajectoryModifier(DotaModifier* modifier) {
			if (!Config::WarnLinearProjectiles)
				return;

			auto owner = modifier->GetOwner();
			if (AbilityTrajectories.count(modifier->GetName()) && 
				!EntityTrajectories.count(modifier)) {
				int offset = AbilityTrajectories[modifier->GetName()];
				EntityTrajectories[modifier] = EntTrajectoryInfo{
					.offset = offset,
					.particleWrap = DrawTrajectory(
					owner->GetPos(),
					owner->GetForwardVector(offset))
				};
			}
		}

		void RemoveParticleIfTrajectoryModifier(DotaModifier* modifier) {
			if (EntityTrajectories.count(modifier)) {
				Globals::ParticleManager->DestroyParticle(EntityTrajectories[modifier].particleWrap);
				EntityTrajectories.erase(modifier);
			}
		}

		void FrameBasedLogic() {
			for (auto& [modifier, info] : EntityTrajectories) {
				auto owner = modifier->GetOwner();
				auto forwardVec = modifier->GetOwner()->GetForwardVector(info.offset);
				info.particleWrap.particle->SetControlPoint(7, &forwardVec);
			}
		}

		void ProcessLinearProjectileMsg(NetMessageHandle_t* msgHandle, google::protobuf::Message* msg) {
			if (msgHandle->messageID == 471) {
				auto linProjMsg = reinterpret_cast<CDOTAUserMsg_CreateLinearProjectile*>(msg);
				auto& newProj =
					linearProjectiles[linProjMsg->handle()] =
					LinearProjectile{
						.source = Interfaces::EntitySystem->GetEntity(linProjMsg->entindex()),
						.handle = linProjMsg->handle(),
						.distance = linProjMsg->distance(),
						.velocity = Vector2(linProjMsg->velocity().x(), linProjMsg->velocity().y()),
						.acceleration = Vector2(linProjMsg->acceleration().x(), linProjMsg->acceleration().y()),
						.origin = Vector3(linProjMsg->origin().x(), linProjMsg->origin().y(), linProjMsg->origin().z())
				};

				if (Config::WarnLinearProjectiles &&
					newProj.source->GetTeam() != ctx.assignedHero->GetTeam()) {

					auto ratio = newProj.distance / newProj.velocity.Length();
					auto endPoint = newProj.origin + (newProj.velocity * ratio);
					TrackedProjectiles[newProj.handle] = DrawTrajectory(newProj.origin, endPoint);
				}
			}
			else if (msgHandle->messageID == 472) {
				auto linProjMsg = reinterpret_cast<CDOTAUserMsg_DestroyLinearProjectile*>(msg);
				auto handle = linProjMsg->handle();
				linearProjectiles.erase(handle);
				if (TrackedProjectiles.count(handle)) {
					Globals::ParticleManager->DestroyParticle(TrackedProjectiles[handle]);
					TrackedProjectiles.erase(handle);
				}
			}
		}

	};
}
namespace Modules {
	Hacks::LinearProjectileWarner LinearProjectileWarner{};
}