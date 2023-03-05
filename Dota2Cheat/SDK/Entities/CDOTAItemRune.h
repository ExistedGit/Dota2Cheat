#pragma once
#include "CBaseEntity.h"
#include "../Enums.h"

class CDOTAItemRune : public CBaseEntity {
public:
	DotaRunes GetRuneType() {
		return Member<DotaRunes>(0x990);
	}
};

