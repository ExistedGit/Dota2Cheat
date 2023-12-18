#pragma once
#include "../../Base/VClass.h"
#include "../../Base/CUtlVector.h"
#include "../../Base/Definitions.h"

class CEconItemView : public VClass {
public:
	FIELD(void*, GetAttributeList, 0x58);
};

struct CAttributeContainer : public VClass {
	IGETTER(CEconItemView, GetItem, 0x68);
};
class CEconWearable : public CBaseEntity {
public:
	IGETTER(CAttributeContainer, GetAttributeManager, 0x978);
};