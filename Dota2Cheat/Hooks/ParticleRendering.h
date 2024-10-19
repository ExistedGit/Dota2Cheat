#pragma once
#include "../CheatSDK/Config.h"

namespace Hooks {
	inline void* oSetRenderingEnabled{};
	
	inline void hkSetRenderingEnabled(CParticleCollection* thisptr, bool state) {
		ORIGCALL(SetRenderingEnabled)(thisptr, state || Config::ParticleMapHack::ShowParticles);
	}
}