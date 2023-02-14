#pragma once
#include "sdk.h"

class IEngineClient :
	public VClass
{
public:
	inline int GetLocalPlayerSlot() {
		int idx = 0;
		CallVFunc<20>(&idx, 0, 0);
		return idx;
	}
	inline bool IsInGame() {
		return (bool)CallVFunc<25, unsigned char>();
	}

};

