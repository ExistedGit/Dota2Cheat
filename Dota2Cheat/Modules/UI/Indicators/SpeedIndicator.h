#pragma once
#include "../../../SDK/pch.h"
#include "../../../CheatSDK/include.h"
#include "../../../Utils/Drawing.h"

namespace Hacks {
	// Created together with Wolf49406
	class SpeedIndicator {
	public:
		void Draw();
	};
}
namespace Modules {
	inline Hacks::SpeedIndicator SpeedIndicator{};
}