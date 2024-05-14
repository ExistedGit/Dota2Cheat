#pragma once
#include "../Base/VClass.h"
#include "../VMI.h"

class CEngineClient :
	public VClass
{
public:
	//uint32_t GetLocalPlayerID() {
	//	uint32_t idx = 0;
	//	CallVFunc<VMI::CEngineClient::GetLocalPlayer>(&idx, 0);
	//	return idx;
	//}

	VGETTER(bool, IsInGame, VMI::CEngineClient::IsInGame);
};

