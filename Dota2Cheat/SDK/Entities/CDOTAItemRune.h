#pragma once
#include "CBaseEntity.h"
#include "../Enums.h"

class CDOTAItemRune : public CBaseEntity {
public:
	GETTER(DotaRunes, GetRuneType, Netvars::C_DOTA_Item_Rune::m_iRuneType);
};

