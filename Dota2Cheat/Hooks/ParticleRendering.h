#pragma once
#include "../Config.h"
#include "../SDK/Globals/Signatures.h"
#include "../SDK/Base/Bytehook.h"

namespace Hooks {
	using CParticleCollection = Signatures::CParticleCollection;

	inline Signatures::CreateParticleCollectionFn oCreateParticleCollection = nullptr;

	typedef void(*SetRenderingEnabledFn)(CParticleCollection*, bool);
	inline SetRenderingEnabledFn oSetRenderingEnabled;
	void hkSetRenderingEnabled(CParticleCollection* thisptr, bool state);

	inline void* SetRenderingEnabled = nullptr;
	CParticleCollection* hkCreateParticleCollection(CNewParticleEffect* thisptr, void* particleMgr, void* unk, void** query, int particleIndex);

}