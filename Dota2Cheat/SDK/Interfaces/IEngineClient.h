#pragma once
#include "../Base/VClass.h"
#include "../VTableIndexes.h"
#include "sol/sol.hpp"

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

	static void BindLua(sol::state& lua) {
		auto type = lua.new_usertype<CEngineClient>("CEngineClient");

		type["GetLocalPlayerID"] = &CEngineClient::GetLocalPlayerID;
		type["IsInGame"] = &CEngineClient::IsInGame;
	}
};

