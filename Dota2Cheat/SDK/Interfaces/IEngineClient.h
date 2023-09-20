#pragma once
#include "../Base/VClass.h"
#include "../VTableIndexes.h"

class CEngineClient :
	public VClass
{
public:
	//uint32_t GetLocalPlayerID() {
	//	uint32_t idx = 0;
	//	CallVFunc<VTableIndexes::CEngineClient::GetLocalPlayer>(&idx, 0);
	//	return idx;
	//}

	VGETTER(bool, IsInGame, VTableIndexes::CEngineClient::IsInGame);
};

