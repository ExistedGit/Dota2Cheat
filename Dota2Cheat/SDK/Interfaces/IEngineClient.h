#pragma once
#include "../Base/VClass.h"
#include "../VMI.h"

class CEngineClient :
	public VClass
{
public:
	uint32_t GetLocalPlayer() {
		uint32_t idx = 0;
		GetVFunc(29)(&idx, 0);
		return idx;
	}

	//VGETTER(bool, IsInGame, VMI::CEngineClient::IsInGame);
};

