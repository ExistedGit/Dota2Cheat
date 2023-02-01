#pragma once
#include "Globals.h"

class RoshanListener : public IGameEventListener2 {
public:
	void DESTROY() override {

	}
	void FireGameEvent(void* event) override {
		int minutes = ((int)Globals::GameRules->GetGameTime() - 90) / 60,
			seconds = ((int)Globals::GameRules->GetGameTime() - 90) % 60;
		std::cout << "Roshan died: " 
			<< (minutes < 10 ? "0" : "") << minutes << ':'
			<< (seconds < 10 ? "0" : "") << seconds << "\n";
	}
};
