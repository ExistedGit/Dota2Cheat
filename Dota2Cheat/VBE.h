#pragma once
#include "Wrappers.h"
#include "UIState.h"
#include "Globals.h"
#include "Config.h"

namespace Hacks {

	// DEPRECATED: on 21.02.2022 Valve announced that m_flStartSequence and maybe something else was their honeypot
	// Cheats accessing it were detected and their users banned. The VBE circus ended, though there must still be the offset to the local player's part of it
	// Search on UnknownCheats, there are at least two cheats using that there(if it still works, that is)
	
	// C_DOTA_BaseNPC's m_startSequenceCycle is set to 0 if you are visible
	// It updates 30 times a second, thus if we check it at 60FPS it needs to be 0 three times in a row
	// This netvar is also briefly zeroed if you order the unit to move to the exact position they stand at,
	// suggesting it's not only responsible for VBE
	class VisibleByEnemy {
	private:
		// After how many ticks with SSC equal to 0 you need to be considered visible
		// Needed to fix the aforementioned false positive when you order your hero to move to the same position
		const int tickTreshold = 5;
		struct TrackingInfo {
			int sscCount = 0;
			float sscSum = 0;
			bool visible = false;
			CDOTAParticleManager::ParticleWrapper vbeParticleWrap{};
		};
		std::map<BaseNpc*, TrackingInfo> TrackedEntities{};
	public:
		void Reset() {
			for (auto& [ent, info] : TrackedEntities)
				if (info.vbeParticleWrap.particle)
					Globals::ParticleManager->DestroyParticle(info.vbeParticleWrap);
			//TrackedEntities.clear();
		}
		void SubscribeEntity(BaseNpc* ent) {
			//TrackedEntities[ent] = TrackingInfo{};
		}
		void CreateVbeParticleFor(BaseNpc* ent) {
			TrackedEntities[ent].vbeParticleWrap = Globals::ParticleManager->CreateParticle(
				"particles/items5_fx/revenant_brooch_ring_glow.vpcf",
				PATTACH_ABSORIGIN_FOLLOW,
				ent
			);

			TrackedEntities[ent].vbeParticleWrap.particle
				->SetControlPoint(0, &Vector3::Zero);
		}

		void FrameBasedLogic() {
			for (auto& pair : TrackedEntities) {

				auto& trackInfo = pair.second;
				trackInfo.sscCount++;
				trackInfo.sscSum += pair.first->GetSSC();
				if (trackInfo.sscCount == tickTreshold) {
					pair.second.visible = trackInfo.sscSum == 0;
					trackInfo.sscCount = trackInfo.sscSum = 0;
					UIState::HeroVisibleToEnemy = pair.second.visible;
				}

				bool vbeParticleActive = trackInfo.vbeParticleWrap.particle;

				if (pair.second.visible && !vbeParticleActive && Config::VBEShowParticle)
					CreateVbeParticleFor(pair.first);
				else if (vbeParticleActive &&
					(!pair.second.visible ||			   // if not visible 
						!Config::VBEShowParticle)) // OR if VBE particle was disabled via config
					Globals::ParticleManager->DestroyParticle(trackInfo.vbeParticleWrap);
				
			}
		}
	};
}

namespace Modules {
	inline Hacks::VisibleByEnemy VBE{};
}