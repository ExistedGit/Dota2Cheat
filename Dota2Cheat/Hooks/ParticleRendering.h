#pragma once
#include "../pch.h"
#include "../CheatSDK/Config.h"

namespace Hooks {
	typedef void(*SetRenderingEnabledFn)(CParticleCollection*, bool);
	inline SetRenderingEnabledFn oSetRenderingEnabled{};
	
	inline void hkSetRenderingEnabled(CParticleCollection* thisptr, bool state) {
		oSetRenderingEnabled(thisptr, state || Config::ParticleMapHack::ShowParticles);
	}
}