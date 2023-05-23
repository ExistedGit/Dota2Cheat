#pragma once
#include "../SDK/pch.h"
#include "../CheatSDK/Lua/LuaModules.h"

namespace Hooks {
	using FireEventClientSideFn = bool(*)(CGameEventManager*, CGameEvent*);
	inline FireEventClientSideFn oFireEventClientSide{};
	inline bool hkFireEventClientSide(CGameEventManager* thisptr, CGameEvent* ev) {
		if (IsValidReadPtr(ev) && ctx.gameStage == GameStage::IN_GAME)
			Lua::CallModuleFunc("OnGameEvent", ev);

		return oFireEventClientSide(thisptr, ev);
	}
}