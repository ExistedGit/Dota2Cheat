#pragma once
#include "../../Base/VClass.h"
#include "../../Base/CUtlVector.h"
#include "../../Base/Definitions.h"
#include "../../Netvars.h"

class CEconItemView : public VClass {
public:
	FIELD(void*, GetAttributeList, Netvars::C_EconItemView::m_AttributeList);
};

struct CAttributeContainer : public VClass {
	IGETTER(CEconItemView, GetItem, 0x68);
};

class CEconWearable : public CBaseEntity {
public:
	IGETTER(CAttributeContainer, GetAttributeManager, Netvars::C_EconEntity::m_AttributeManager);
};