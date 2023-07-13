#pragma once
#include "../pch.h"
#include "../MatchStateHandling.h"

// CDOTAGameRules::m_nGameState change

class dota_game_state_change_l : public IGameEventListener2 {
public:
	void FireGameEvent(CGameEvent* event) override {
		DOTA_GameState state = (DOTA_GameState)event->GetInt("new_state", -1);
		MatchStateManager.OnStateChanged(state);
	}
};