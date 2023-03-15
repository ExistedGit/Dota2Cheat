#include "ParticleRendering.h"
using CParticleCollection = Signatures::CParticleCollection;

void Hooks::hkSetRenderingEnabled(CParticleCollection* thisptr, bool state) {
	bool FakeState = state;

	if (Config::RenderAllParticles)
		FakeState = true;

	oSetRenderingEnabled(thisptr, FakeState);
}

CParticleCollection* Hooks::hkCreateParticleCollection(CNewParticleEffect* thisptr, void* particleMgr, void* unk, void** query, int particleIndex) {
	CParticleCollection* Ret = oCreateParticleCollection(thisptr, particleMgr, unk, query, particleIndex);

	if (!SetRenderingEnabled) {
		SetRenderingEnabled = getvfunc(thisptr->Member<VClass*>(0x20), 95).ptr;
		HookFunc(SetRenderingEnabled, hkSetRenderingEnabled, &oSetRenderingEnabled, "SetRenderingEnabled");
	}

	return Ret;
}
