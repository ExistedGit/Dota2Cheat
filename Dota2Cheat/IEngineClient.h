#pragma once
#include "sdk.h"

class IEngineClient :
	public VClass
{
public:
	int GetLocalPlayerSlot() {
		int idx = 0;
		CallVFunc<20>(&idx, 0, 0);
		return idx;
	}
	bool IsInGame() {
		return (bool)CallVFunc<25, unsigned char>();
	}

};

