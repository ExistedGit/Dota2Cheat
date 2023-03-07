#include "IEngineClient.h"

int IEngineClient::GetLocalPlayerSlot() {
	int idx = 0;
	CallVFunc<20>(&idx, 0, 0);
	return idx;
}

bool IEngineClient::IsInGame() {
	return (bool)CallVFunc<25, unsigned char>();
}

void IEngineClient::BindLua(sol::state& lua) {
	auto type = lua.new_usertype<IEngineClient>("IEngineClient");
	type["GetLocalPlayerSlot"] = &IEngineClient::GetLocalPlayerSlot;
	type["IsInGame"] = &IEngineClient::IsInGame;
}
