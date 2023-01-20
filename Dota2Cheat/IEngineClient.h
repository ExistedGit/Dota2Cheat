#pragma once
#include "sdk.h"

class IEngineClient :
	public VClass
{
public:
	inline int GetLocalPlayerSlot() {
		int idx = 0;
		GetVFunc(20)(this, &idx, 0, 0);
		return idx;
	}
	inline bool IsInGame() {
		return (bool)this->GetVFunc(25).Execute<unsigned char>(this);
	}

};

