#pragma once
#include "../Config.h"
#include "../SDK/Globals/Signatures.h"
#include "../SDK/Base/Bytehook.h"

namespace Hooks {
	using CParticleCollection = Signatures::CParticleCollection;

	inline void* SetRenderingEnabled = nullptr;
	inline Signatures::CreateParticleCollectionFn oCreateParticleCollection = nullptr;

	typedef void(*SetRenderingEnabledFn)(CParticleCollection*, bool);
	inline SetRenderingEnabledFn oSetRenderingEnabled;
	void hkSetRenderingEnabled(CParticleCollection* thisptr, bool state);

	CParticleCollection* hkCreateParticleCollection(CNewParticleEffect* thisptr, void* particleMgr, void* unk, void** query, int particleIndex);

}