#pragma once
#include "../../SDK/pch.h"

namespace Particles {

	class ParticleGarbageCollector {
	private:
		struct ParticleGCInfo {
			float assignTime;
			float dieTime{};
			ParticleWrapper particleWrap;
		};
		std::vector<ParticleGCInfo> gcInfo;
	public:

		void SetDieTime(ParticleWrapper particle, float dieTime) {
			gcInfo.push_back(
				ParticleGCInfo{
				.assignTime = GameSystems::GameRules->GetGameTime(),
				.dieTime = dieTime,
				.particleWrap = particle
				}
			);
		}
		void RemoveFromGC(ParticleWrapper particle) {
			decltype(gcInfo)::iterator foundIterator = gcInfo.end();
			for (auto it = gcInfo.begin(); it != gcInfo.end(); ++it)
				if ((*it).particleWrap.particle == particle.particle) {
					foundIterator = it;
					break;
				}
			if (foundIterator != gcInfo.end())
				gcInfo.erase(foundIterator);
		}
		void FrameBasedLogic() {
			for (auto& info : gcInfo)
				if (info.assignTime + info.dieTime <= GameSystems::GameRules->GetGameTime())
					GameSystems::ParticleManager->DestroyParticle(info.particleWrap);
		}
	};
}
namespace Modules {
	inline Particles::ParticleGarbageCollector ParticleGC{};
}