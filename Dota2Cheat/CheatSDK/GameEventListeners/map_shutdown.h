#pragma once
#include "../pch.h"
#include "../MatchStateHandling.h"

// Gets fired when exiting a match

class map_shutdown_l : public IGameEventListener2 {
public:
	void FireGameEvent(CGameEvent* ev) override {
		MatchStateManager.LeftMatch();
	}
};