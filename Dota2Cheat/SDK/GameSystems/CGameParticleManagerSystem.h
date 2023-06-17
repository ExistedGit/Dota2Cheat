#pragma once
#include "../Base/VClass.h"
#include "../Base/Definitions.h"
#include "CDOTAParticleManager.h"
class CGameParticleManagerSystem : public VClass {
public:
	GETTER(CDOTAParticleManager*, GetParticleManager, 0x18);
};