#pragma once
#include "../Base/VClass.h"
#include "../VTableIndexes.h"

class CEngineClient :
	public VClass
{
public:
	DWORD GetLocalPlayerID() {
		DWORD idx = 0;
		CallVFunc<VTableIndexes::CEngineClient::GetLocalPlayer>(&idx, 0);
		return idx;
	}

	VGETTER(bool, IsInGame, VTableIndexes::CEngineClient::IsInGame);
};

