#pragma once
#include "../pch.h"
#include "../Modules/Hacks/AegisSnatcher.h"
#include "MatchStateHandling.h"
#include <format>

class RoshanListener : public IGameEventListener2 {
public:
	void DESTROY() override {

	}
	void FireGameEvent(CGameEvent* ev) override {
#ifdef _DEBUG

		int gameTime = GameSystems::GameRules->GetGameTime(),
			offset = 0,
			respTimeMin = 0, respTimeMax = 0;

		switch (GameSystems::GameRules->GetGameMode()) {
		case DOTA_GAMEMODE_TURBO: offset = 60; respTimeMin = 5; respTimeMax = 6; break;
		case DOTA_GAMEMODE_AP: case DOTA_GAMEMODE_ALL_DRAFT: offset = 90; respTimeMin = 8; respTimeMax = 11; break;
		}

		int minutes = (gameTime - offset) / 60,
			seconds = (gameTime - offset) % 60;

		Interfaces::InputService->CmdCommand(std::format("say_team Roshan respawn: {:02}:{:02}-{:02}:{:02}",
			minutes + respTimeMin,
			seconds,
			minutes + respTimeMax,
			seconds
		));
		LogF(LP_INFO, "Roshan died: {:02}:{:02}", minutes, seconds);
#endif // _DEBUG
	}
};
