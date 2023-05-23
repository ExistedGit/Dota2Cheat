#pragma once
#include "../SDK/pch.h"
#include "../CheatSDK/Config.h"

namespace Hooks {
	using CParticleCollection = Signatures::CParticleCollection;

	typedef void(*SetRenderingEnabledFn)(CParticleCollection*, bool);
	inline SetRenderingEnabledFn SetRenderingEnabled{};
	inline SetRenderingEnabledFn oSetRenderingEnabled{};
	
	inline void hkSetRenderingEnabled(CParticleCollection* thisptr, bool state) {
		oSetRenderingEnabled(thisptr, state || Config::RenderAllParticles);
	}
}