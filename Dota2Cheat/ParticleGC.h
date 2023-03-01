#pragma once
#include "Globals.h"

namespace Particles {
		
	class ParticleGarbageCollector {
	private:
		struct ParticleGCInfo {
			float assignTime;
			float dieTime{};
			CDOTAParticleManager::ParticleWrapper particleWrap;
		};
		std::vector<ParticleGCInfo> gcInfo;
	public:

		void SetDieTime(CDOTAParticleManager::ParticleWrapper particle, float dieTime) {
			gcInfo.push_back(
				ParticleGCInfo{
				.assignTime = Globals::GameRules->GetGameTime(),
				.dieTime = dieTime,
				.particleWrap = particle
				}
			);
		}
		void RemoveFromGC(CDOTAParticleManager::ParticleWrapper particle) {
			decltype(gcInfo)::iterator foundIterator = gcInfo.end();
			for (auto it = gcInfo.begin(); it != gcInfo.end(); ++it)
				if ((*it).particleWrap.particle == particle.particle) {
					foundIterator = it;
					break;
				}
			if(foundIterator != gcInfo.end())
			gcInfo.erase(foundIterator);
		}
		void FrameBasedLogic() {
			for (auto& info : gcInfo)
				if (info.assignTime + info.dieTime <= Globals::GameRules->GetGameTime())
					Globals::ParticleManager->DestroyParticle(info.particleWrap);
		}
	};
}
namespace Modules {
	Particles::ParticleGarbageCollector ParticleGC{};
}