#pragma once
#include "../Base/CUtlVector.h"
#include "../Base/VClass.h"
#include "../Base/Definitions.h"
#include "../Base/Vector.h"

struct MinimapObject {
	const char
		* type,
		* additionalInfo{};
	Vector position{};
	uint32_t unk4, unk0 = 0xFFFFFFFF, unk3;
	uint32_t entIdx, unk1{};
	void* unk2[2];
	uint32_t unk5, unk6 = 16777216;
};

class CDOTA_MinimapObjectManager : public VClass
{
public:
	GETTER(CUtlVector<MinimapObject>, GetMinimapObjects, 0x18);
};

