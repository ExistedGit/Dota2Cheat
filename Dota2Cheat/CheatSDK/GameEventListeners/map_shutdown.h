#pragma once
#include "../SDK/pch.h"
#include "../MatchStateHandling.h"

class map_shutdown_l : public IGameEventListener2 {
public:
	void DESTROY() override {

	}
	void FireGameEvent(CGameEvent* ev) override {
		MatchStateManager.LeftMatch();
	}
};