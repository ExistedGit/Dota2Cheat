#pragma once
#include "../../SDK/pch.h"
#include "../../Config.h"

namespace Hacks {
	class TrueSightESP {

		std::map<CDOTAModifier*, ParticleWrapper> TrackedModifiers{};
	public:
		void RemoveParticleIfTrueSight(CDOTAModifier* modifier);

		void DrawParticleIfTrueSight(CDOTAModifier* modifier);

		void OnDisable() {
			for (auto& [_, wrap] : TrackedModifiers) {
				GameSystems::ParticleManager->DestroyParticle(wrap);
			}
		}
	};
}

namespace Modules {
	inline Hacks::TrueSightESP TrueSightESP{};
}