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

		void SetDieTime(ParticleWrapper particle, float dieTime);
		void RemoveFromGC(ParticleWrapper particle);
		void FrameBasedLogic();
		void Reset();
	};
}
namespace Modules {
	inline Particles::ParticleGarbageCollector ParticleGC{};
}