#pragma once
#include "../Base/VClass.h"
#include "../Base/Definitions.h"
#include "../Base/CUtlVector.h"
#include "../Entities/CBaseEntity.h"

class CDOTA_BinaryObjectSystem : public VClass {
public:
	GETTER(CUtlVector<CBaseEntity*>, GetTrees, 0x18);
};