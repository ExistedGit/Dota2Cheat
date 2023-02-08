#pragma once
#include "Wrappers.h"
#include "UIState.h"
#include "Globals.h"
#include "Config.h"

namespace Hacks {
	class VisibleByEnemy {
	private:
		int sscCount = 0;
		float sscSum = 0;

		CDOTAParticleManager::ParticleWrapper vbeParticleWrap{};
		bool visible = false;
	public:
		inline void CreateVbeParticleFor(BaseEntity* ent) {
			vbeParticleWrap = Globals::ParticleManager->CreateParticle(
				"particles/items5_fx/revenant_brooch_ring_glow.vpcf",
				CDOTAParticleManager::ParticleAttachment_t::PATTACH_ABSORIGIN_FOLLOW,
				ent
			);

			vbeParticleWrap.particle
				->SetControlPoint(0, &Vector3::Zero);
		}
		inline void Reset() {
			if (vbeParticleWrap.particle)
				Globals::ParticleManager->DestroyParticle(vbeParticleWrap);
		}
		inline void FrameBasedLogic() {
			sscCount++;
			sscSum += assignedHero->GetSSC();
			if (sscCount == 3) {
				visible = sscSum == 0;
				sscCount = sscSum = 0;
				UIState::HeroVisibleToEnemy = visible;
			}


			bool vbeParticleActive = vbeParticleWrap.particle != nullptr;

			if (visible && !vbeParticleActive && Config::VBEShowParticle)
				CreateVbeParticleFor(assignedHero);

			else if ((!visible && vbeParticleActive) || // if not visible and there's a particle
				(!Config::VBEShowParticle && vbeParticleWrap.particle)) // OR if VBE particle was disabled via config
				Globals::ParticleManager->DestroyParticle(vbeParticleWrap);
		}
	};
}

namespace Modules {
	Hacks::VisibleByEnemy VBE{};
}