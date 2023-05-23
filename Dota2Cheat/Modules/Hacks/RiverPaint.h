#pragma once
#include "../../SDK/pch.h"
#include "../../CheatSDK/Config.h"

namespace Hacks {
	// River paint is controlled by a hidden ConVar
	class RiverPaint {
	public:
		void FrameBasedLogic() {
			static auto cvar = CVarSystem::CVars["dota_river_type"];
			cvar.m_pVar->value.ui32 = Config::Changer::RiverListIdx;
		}
	};
}

namespace Modules {
	inline Hacks::RiverPaint RiverPaint{};
}