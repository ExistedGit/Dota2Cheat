#pragma once
#include "../../pch.h"
#include "../../CheatSDK/include.h"

namespace Modules {
	inline class M_TrueSightESP {
		std::map<CDOTAModifier*, ParticleWrapper> TrackedModifiers{};
	public:
		void OnModifierRemoved(CDOTAModifier* modifier);

		void OnModifierAdded(CDOTAModifier* modifier);

		void OnDisabled() {
			for (auto& [_, wrap] : TrackedModifiers) {
				GameSystems::ParticleManager->DestroyParticle(wrap);
			}
		}
	} TrueSightESP{};
}