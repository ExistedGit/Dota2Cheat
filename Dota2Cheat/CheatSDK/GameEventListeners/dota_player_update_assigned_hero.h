#pragma once
#include "GameSystems/CGameEventManager.h"

class dota_player_update_assigned_hero_l : public IGameEventListener2 {
public:
	void FireGameEvent(CGameEvent* ev) override;
};