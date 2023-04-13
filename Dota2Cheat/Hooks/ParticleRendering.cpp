#include "ParticleRendering.h"
#include "../Modules/Hacks/DotaPlusUnlocker.h"
#include "../Config.h"

using CParticleCollection = Signatures::CParticleCollection;

void Hooks::hkSetRenderingEnabled(CParticleCollection* thisptr, bool state) {
	oSetRenderingEnabled(thisptr, state || Config::RenderAllParticles);
}

// An old Wolf49406-style hook
// 
//CParticleCollection* Hooks::hkCreateParticleCollection(CNewParticleEffect* thisptr, void* particleMgr, void* unk, void** query, int particleIndex) {
//	CParticleCollection* Ret = oCreateParticleCollection(thisptr, particleMgr, unk, query, particleIndex);
//
//	if (!SetRenderingEnabled) {
//		SetRenderingEnabled = getvfunc(thisptr->Member<VClass*>(0x20), 95).ptr;
//		HOOKFUNC(SetRenderingEnabled);
//	}
//
//	return Ret;
//}
