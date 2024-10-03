#pragma once
#include "../pch.h"
#include "../Data/EntityList.h"

class dota_on_hero_finish_spawn_l : public IGameEventListener2 {
public:
	void FireGameEvent(CGameEvent* event) override {}
};