#pragma once
#include "../SDK/pch.h"

struct LuaListener : public IGameEventListener2 {
	sol::function func;

	void DESTROY() override {

	}

	void FireGameEvent(CGameEvent* ev) override {
		if (func.valid())
			func(ev);
	}
};