#pragma once
#include "../Base/VClass.h"
#include "../Base/Definitions.h"

class CDOTAParticleManager;

class CGameParticleManagerSystem : public VClass {
public:
	// Offset mentioned all over its vfuncs
	GETTER(CDOTAParticleManager*, GetParticleManager, 0x18);

	static CGameParticleManagerSystem* Get();
};