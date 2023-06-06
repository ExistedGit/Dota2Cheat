#pragma once
#include "../SDK/pch.h"
#include "../MatchStateHandling.h"

class dota_game_state_change_l : public IGameEventListener2 {
public:
	void DESTROY() override {}

	void FireGameEvent(CGameEvent* event) override {
		DOTA_GameState state = (DOTA_GameState)event->GetInt("new_state", -1);
		MatchStateManager.OnStateChanged(state);
	}
};