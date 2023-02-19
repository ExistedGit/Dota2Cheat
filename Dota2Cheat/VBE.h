#pragma once
#include "Wrappers.h"
#include "UIState.h"
#include "Globals.h"
#include "Config.h"

namespace Hacks {
	// C_DOTA_BaseNPC's m_startSequenceCycle is set to 0 if you are visible
	// It updates 30 times a second, thus if we check it at 60FPS it needs to be 0 three times in a row
	// This netvar is also briefly zeroed if you order the unit to move to the exact position they stand at,
	// suggesting it's not only responsible for VBE
	class VisibleByEnemy {
	private:
		int sscCount = 0;
		float sscSum = 0;
		
		// After how many ticks with SSC equal to 0 you need to be considered visible
		// Needed to fix the aforementioned false positive when you order your hero to more to the same position
		const int tickTreshold = 5; 

		CDOTAParticleManager::ParticleWrapper vbeParticleWrap{};
		bool visible = false;
	public:
		void CreateVbeParticleFor(BaseEntity* ent) {
			vbeParticleWrap = Globals::ParticleManager->CreateParticle(
				"particles/items5_fx/revenant_brooch_ring_glow.vpcf",
				CDOTAParticleManager::ParticleAttachment_t::PATTACH_ABSORIGIN_FOLLOW,
				ent
			);

			vbeParticleWrap.particle
				->SetControlPoint(0, &Vector3::Zero);
		}
		void Reset() {
			if (vbeParticleWrap.particle)
				Globals::ParticleManager->DestroyParticle(vbeParticleWrap);
		}
		void FrameBasedLogic() {
			sscCount++;
			sscSum += assignedHero->GetSSC();
			if (sscCount == tickTreshold) {
				visible = sscSum == 0;
				sscCount = sscSum = 0;
				UIState::HeroVisibleToEnemy = visible;
			}


			bool vbeParticleActive = vbeParticleWrap.particle ;

			if (visible && !vbeParticleActive && Config::VBEShowParticle)
				CreateVbeParticleFor(assignedHero);

			else if (vbeParticleActive &&  
				(!visible ||			   // if not visible 
				!Config::VBEShowParticle)) // OR if VBE particle was disabled via config
				Globals::ParticleManager->DestroyParticle(vbeParticleWrap);
		}
	};
}

namespace Modules {
	inline Hacks::VisibleByEnemy VBE{};
}