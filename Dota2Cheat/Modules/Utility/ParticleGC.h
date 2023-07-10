#pragma once
#include "../../SDK/pch.h"

namespace Modules {
	inline class M_ParticleGarbageCollector {
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
			void OnFrame();
			void Reset();
	} ParticleGC;
}