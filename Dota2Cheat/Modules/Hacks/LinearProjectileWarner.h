#pragma once
#include "../../pch.h"
#include "../../SDK/Protobufs/dota_usermessages.pb.h"
#include "../../CheatSDK/include.h"
#include "../MListeners.h"

// particles\ui_mouseactions\range_finder_tower_aoe.vpcf
// Point 0: nothing
// Point 2: coord of the second end of the dashed line
// Point 3: X is boolean, controls some point in the center
// Point 4: color of that point
// Point 6: X is boolean, controls visibility of the particle
// Point 7: coord of the marker and the first end of the dashed line


// particles/ui_mouseactions/range_finder_tower_line.vpcf
// is the owner of control points 2, 6, 7

namespace Modules {
	// Shows linear projectile trajectories with a red line
	// Also shows potential trajectories for aiming heroes like Hoodwink
	inline class M_LinearProjectileWarner : public IFrameListener, public INetChanListener {
	public:

		struct LinearProjectile {
			CBaseEntity* source{};

			int32_t handle{};
			float distance{};
			Vector2D velocity{ 0, 0 };
			Vector2D acceleration{ 0, 0 };
			Vector origin{};
		};

		std::map<int32_t, ParticleWrapper> TrackedProjectiles{};
		std::map<int32_t, LinearProjectile> linearProjectiles{};


		struct AbilityTrajectoryConfig {
			int value;
			bool isAbilitySlot; // if false, value is distance
		};

		std::map<std::string_view, AbilityTrajectoryConfig> AbilityTrajectories{
			{"modifier_hoodwink_sharpshooter_windup",
			AbilityTrajectoryConfig{
				.value = 3125,
				.isAbilitySlot = false
			}
			},
			{"modifier_keeper_of_the_light_illuminate",
		AbilityTrajectoryConfig{
				.value = 7,
				.isAbilitySlot = true
			}},
			{
				"modifier_primal_beast_onslaught_windup",
				AbilityTrajectoryConfig{
				.value = 2000,
				.isAbilitySlot = false
			}}
		};

		struct EntTrajectoryInfo {
			int offset;
			ParticleWrapper particleWrap;
		};

		std::map<CDOTAModifier*, EntTrajectoryInfo> EntityTrajectories{};

		// Draws a dashed red line from begin to end
		// Returns the wrapper for the created particle
		ParticleWrapper DrawTrajectory(const Vector& begin, const Vector& end);


		void OnModifierAdded(CDOTAModifier* modifier);
		void OnFrame() override;
		void OnModifierRemoved(CDOTAModifier* modifier);

		void OnReceivedMsg(NetMessageHandle_t* msgHandle, google::protobuf::Message* msg) override;

	} LinearProjectileWarner;
}