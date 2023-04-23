#pragma once
#include "../CheatSDK/Config.h"

namespace Hooks {
	using CParticleCollection = Signatures::CParticleCollection;

	typedef void(*SetRenderingEnabledFn)(CParticleCollection*, bool);
	inline SetRenderingEnabledFn SetRenderingEnabled = nullptr;
	//inline Signatures::CreateParticleCollectionFn oCreateParticleCollection = nullptr;

	inline SetRenderingEnabledFn oSetRenderingEnabled;
	void hkSetRenderingEnabled(CParticleCollection* thisptr, bool state);

	//CParticleCollection* hkCreateParticleCollection(CNewParticleEffect* thisptr, void* particleMgr, void* unk, void** query, int particleIndex);
}