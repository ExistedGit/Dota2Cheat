#pragma once
#include <map>
#include "../Utility/ParticleGC.h"
#include "../../CheatSDK/include.h"

#include "../MListeners.h"

namespace Modules {
inline 
	class ParticleAbilityWarner : public INetChanListener {
		// Draws a dashed red line from begin to end
		// Returns the wrapper for the created particle
		ParticleWrapper DrawTrajectory(Vector begin, Vector end);
		ParticleWrapper DrawRadius(Vector pos, float radius);
		enum AbilityParticles : uint64_t {
			AP_PUDGE_MEAT_HOOK = 16517413739925325824,
			AP_KOTL_ILLUMINATE = 2498180139172148061,
			AP_BANSHEE_CRYPT_SWARM = 3020948711683823655,
			AP_MEEPO_EARTHBIND = 9143050083230003323
		};

		struct AbilityParticleInfo {
			AbilityParticles nameIndex;
			Vector begin{}, end{};
			CDOTABaseNPC* owner;
		};
		// Map of particle name indexes to their respective ability indexes
		const std::map<AbilityParticles, uint32_t> AbilityIndexes{
			{AP_KOTL_ILLUMINATE, 7},	// illuminate is moved to index 7 when End Illuminate takes its place
			{AP_BANSHEE_CRYPT_SWARM, 0}
		};

		struct TrackedParticlesData {
			ParticleWrapper trajectory{};
			ParticleWrapper aoe{};
		};
		std::map<uint32_t, AbilityParticleInfo> queuedParticleIndexes{};
		std::map<uint32_t, TrackedParticlesData> TrackedAbilityParticles{};
	public:
		// Meat hook:
		// On create: begin = fallback_position 
		// Update: end = control point 1

		// Illuminate, Crypt Swarm:
		// Update: velocity = control point 1

		void OnReceivedMsg(NetMessageHandle_t* msgHandle, google::protobuf::Message* msg) override;
	} ParticleAbilityWarner;
}