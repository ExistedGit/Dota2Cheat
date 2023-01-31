#pragma once
#include "Globals.h"

class RoshanListener : public IGameEventListener2 {
public:
	void DESTROY() override {

	}
	void FireGameEvent(void* event) override {
		std::cout << "Roshan died: " 
			<< (int)Globals::GameRules->GetGameTime() / 60 << ':' 
			<< (Globals::GameRules->GetGameTime() - (int)Globals::GameRules->GetGameTime()) * 60 << "\n";
	}
};
