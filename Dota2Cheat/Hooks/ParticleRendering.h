#pragma once
#include "../Config.h"
#include "../SDK/Globals/Signatures.h"
#include "../SDK/Base/Bytehook.h"

namespace Hooks {
	using CParticleCollection = Signatures::CParticleCollection;

	inline Signatures::CreateParticleCollectionFn oCreateParticleCollection = nullptr;

	typedef void(*SetRenderingEnabledFn)(CParticleCollection*, bool);
	inline SetRenderingEnabledFn oSetRenderingEnabled;
	void hkSetRenderingEnabled(CParticleCollection* thisptr, bool state) {
		bool FakeState = state;

		if (Config::RenderAllParticles)
			FakeState = true;

		oSetRenderingEnabled(thisptr, FakeState);
	}

	inline void* SetRenderingEnabled = nullptr;
	CParticleCollection* hkCreateParticleCollection(CNewParticleEffect* thisptr, void* particleMgr, void* unk, void* query, bool bUnk, float fUnk, int particleIndex) {
		CParticleCollection* Ret = oCreateParticleCollection(thisptr, particleMgr, unk, query, bUnk, fUnk, particleIndex);

		if (!SetRenderingEnabled) {
			SetRenderingEnabled = getvfunc(thisptr->Member<VClass*>(0x20), 95).ptr;
			HookFunc(SetRenderingEnabled, hkSetRenderingEnabled, &oSetRenderingEnabled, "SetRenderingEnabled");
		}

		return Ret;
	}

}