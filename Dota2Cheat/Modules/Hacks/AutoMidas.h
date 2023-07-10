#pragma once
#include "../../SDK/pch.h"
#include "../../CheatSDK/include.h"

namespace Modules {
inline 
	// Automatically uses Hand of Midas on non-ancient creeps
	// Compares XP bounty for them to the config setting
	class AutoMidas {
	public:
		const float usePeriod = 0.6f;
		float lastTime = 0;
		void OnFrame();
	} AutoMidas;
}