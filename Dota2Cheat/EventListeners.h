#pragma once
#include "Globals.h"
#include <format>

class RoshanListener : public IGameEventListener2 {
public:
	int gameStartTime = 0;
	void DESTROY() override {

	}
	void FireGameEvent(void* event) override {
		if (gameStartTime <= 4)
			gameStartTime = 0;

		int gameTime = Globals::GameRules->GetGameTime();
		int minutes = (gameTime - gameStartTime) / 60,
			seconds = (gameTime - gameStartTime) % 60;

		Interfaces::InputService->CmdCommand(std::format("say_team Roshan died: {}{}:{}{}",
			(minutes < 10 ? "0" : ""),
			minutes,
			(seconds < 10 ? "0" : ""),
			seconds).c_str());
		std::cout << std::dec << "Roshan died: "
			<< (minutes < 10 ? "0" : "") << minutes << ':'
			<< (seconds < 10 ? "0" : "") << seconds << "\n";
	}
};
