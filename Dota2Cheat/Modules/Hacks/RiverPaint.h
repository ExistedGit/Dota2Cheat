#pragma once
#include "../../SDK/pch.h"
#include "../../CheatSDK/Config.h"

namespace Hacks {
	// River paint is controlled by a hidden ConVar
	class RiverPaint {
	public:
		void FrameBasedLogic();
	};
}

namespace Modules {
	inline Hacks::RiverPaint RiverPaint{};
}