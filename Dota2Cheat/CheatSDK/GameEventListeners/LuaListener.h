#pragma once
#include "../SDK/pch.h"

struct LuaListener : public IGameEventListener2 {
	sol::function func;
	sol::table luaModule;

	void DESTROY() override {

	}

	void FireGameEvent(CGameEvent* ev) override {
		if (func.valid())
			func(luaModule, ev);
	}
};