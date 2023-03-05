#pragma once
#include "../Base/VClass.h"
#include "sol/sol.hpp"

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
	
	static void BindLua(sol::state& lua) {
		auto type = lua.new_usertype<IEngineClient>("IEngineClient");
		type["GetLocalPlayerSlot"] = &IEngineClient::GetLocalPlayerSlot;
		type["IsInGame"] = &IEngineClient::IsInGame;
	}
};

