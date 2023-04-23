#pragma once
#include "../SDK/pch.h"
#include "../CheatSDK/Lua/LuaModules.h"
using FireEventClientSideFn = bool(*)(CGameEventManager*, CGameEvent* );
inline FireEventClientSideFn oFireEventClientSide{};
inline bool hkFireEventClientSide(CGameEventManager* thisptr, CGameEvent* ev) {
	Lua::CallModuleFunc("OnGameEvent", ev);
	return oFireEventClientSide(thisptr, ev);
}