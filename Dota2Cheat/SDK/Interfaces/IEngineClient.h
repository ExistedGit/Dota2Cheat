#pragma once
#include "../Base/VClass.h"
#include "../VTableIndexes.h"
#include "sol/sol.hpp"

class CEngineClient :
	public VClass
{
public:
	int GetLocalPlayerSlot() {
		int idx = 0;
		CallVFunc<VTableIndexes::CEngineClient::GetLocalPlayer>(&idx, 0, 0);
		return idx;
	}
	bool IsInGame() {
		return (bool)CallVFunc<VTableIndexes::CEngineClient::IsInGame, unsigned char>();
	}
	
	static void BindLua(sol::state& lua) {
		auto type = lua.new_usertype<CEngineClient>("CEngineClient");
		type["GetLocalPlayerSlot"] = &CEngineClient::GetLocalPlayerSlot;
		type["IsInGame"] = &CEngineClient::IsInGame;
	}
};

