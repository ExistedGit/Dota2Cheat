#pragma once
#include "../pch.h"
#include "../MatchStateHandling.h"
#include <GameSystems/CGameEventManager.h>
#include <GameSystems/C_DOTAGameRules.h>

#include "../Aliases.h"

// CDOTAGameRules::m_nGameState change

class game_rules_state_change_l : public IGameEventListener2 {
public:
	void FireGameEvent(CGameEvent* event) override {
		if (!CGameRules::Get()) return;
		MatchStateManager.OnStateChanged(CGameRules::Get()->GetGameState());
	}
};