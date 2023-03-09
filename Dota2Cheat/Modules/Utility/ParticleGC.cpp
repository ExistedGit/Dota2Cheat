#include "ParticleGC.h"

void Particles::ParticleGarbageCollector::SetDieTime(ParticleWrapper particle, float dieTime) {
	gcInfo.push_back(
		ParticleGCInfo{
			.assignTime = GameSystems::GameRules->GetGameTime(),
			.dieTime = dieTime,
			.particleWrap = particle
		}
	);
}

void Particles::ParticleGarbageCollector::RemoveFromGC(ParticleWrapper particle) {
	decltype(gcInfo)::iterator foundIterator = gcInfo.end();
	for (auto it = gcInfo.begin(); it != gcInfo.end(); ++it)
		if ((*it).particleWrap.particle == particle.particle) {
			foundIterator = it;
			break;
		}
	if (foundIterator != gcInfo.end())
		gcInfo.erase(foundIterator);
}

void Particles::ParticleGarbageCollector::FrameBasedLogic() {
	for (auto it = gcInfo.begin(); it != gcInfo.end(); )
	{
		if (HVALID(it->particleWrap.handle)) {
			auto& info = *it;
			if (info.assignTime + info.dieTime <= GameSystems::GameRules->GetGameTime())
				GameSystems::ParticleManager->DestroyParticle(info.particleWrap);
			++it;
		} else if (!HVALID(it->particleWrap.handle)) {
			it = gcInfo.erase(it);
		}
	}
}

void Particles::ParticleGarbageCollector::Reset() {
	gcInfo.clear();
}
