#pragma once
#include "../pch.h"
#include "../MatchStateHandling.h"

// CDOTAGameRules::m_nGameState change

class dota_game_state_change_l : public IGameEventListener2 {
public:
	void FireGameEvent(CGameEvent* event) override {
		static auto hash = GetHash("new_state");
		DOTA_GameState state = (DOTA_GameState)event->GetInt(&hash, -1);
		MatchStateManager.OnStateChanged(state);
	}
};