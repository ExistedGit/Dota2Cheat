#pragma once
#include "../pch.h"
#include "../Modules/Hacks/AegisSnatcher.h"
#include "MatchStateHandling.h"
#include <format>

class RoshanListener : public IGameEventListener2 {
public:
	int gameStartTime = 0;
	void DESTROY() override {

	}
	void FireGameEvent(CGameEvent* ev) override {
#ifdef _DEBUG
		if (gameStartTime <= 4)
			gameStartTime = 0;

		int gameTime = GameSystems::GameRules->GetGameTime();

		int offset = 0;
		int respTimeMin = 0, respTimeMax = 0;

		switch (GameSystems::GameRules->GetGameMode()) {
		case DOTA_GAMEMODE_TURBO: offset = 60; respTimeMin = 5; respTimeMax = 6; break;
		case DOTA_GAMEMODE_AP: case DOTA_GAMEMODE_ALL_DRAFT: offset = 90; respTimeMin = 8; respTimeMax = 11; break;
		}

		int minutes = (gameTime - gameStartTime - offset) / 60,
			seconds = (gameTime - gameStartTime - offset) % 60;



		//Interfaces::InputService->CmdCommand(std::format("say_team Roshan died: {}{}:{}{}",
		//	(minutes < 10 ? "0" : ""),
		//	minutes,
		//	(seconds < 10 ? "0" : ""),
		//	seconds).c_str());
		Interfaces::InputService->CmdCommand(std::format("say_team Roshan respawn: {}{}:{}{}-{}{}:{}{}",

			(minutes + respTimeMin < 10 ? "0" : ""),
			minutes + respTimeMin,
			(seconds < 10 ? "0" : ""),
			seconds,

			(minutes + respTimeMax < 10 ? "0" : ""),
			minutes + respTimeMax,
			(seconds < 10 ? "0" : ""),
			seconds

		).c_str());
		std::cout << std::dec << "Roshan died: "
			<< (minutes < 10 ? "0" : "") << minutes << ':'
			<< (seconds < 10 ? "0" : "") << seconds << "\n";
#endif // _DEBUG
	}
};
