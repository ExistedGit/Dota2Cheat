#pragma once
#include "../../pch.h"
#include "../../CheatSDK/include.h"
#include "../MListeners.h"
namespace Modules {
inline 
	// Automatically uses Hand of Midas on non-ancient creeps
	// Compares XP bounty for them to the config setting
	class M_AutoMidas : public IFrameListener{
	public:
		const float usePeriod = 0.6f;
		float lastTime = 0;
		void OnFrame() override;
	} AutoMidas;
}