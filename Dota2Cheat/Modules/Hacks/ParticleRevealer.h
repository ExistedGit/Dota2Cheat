#pragma once
#include "../../SDK/pch.h"
#include "../../CheatSDK/include.h"

namespace Modules {
	inline class M_ParticleRevealer {

	public:
		void TickBasedLogic() {
			for (auto& particle : GameSystems::ParticleManager->GetParticles())
			{
				particle->GetParticle()->GetParticleCollection()->SetRenderingEnabled(true);
			};
		}
	} ParticleRevealer{};
}