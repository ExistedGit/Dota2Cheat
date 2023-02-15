#pragma once

#include "ConVar.h"
#include "Config.h"

namespace Hacks {
	// River paint is controlled by a hidden ConVar
	class RiverPaint {
	public:
		void FrameBasedLogic() {
			static auto cvar = CVarSystem::CVar["dota_river_type"];
			cvar.var->value.ui32 = Config::RiverListIdx;
		}
	};
}

namespace Modules {
	Hacks::RiverPaint RiverPaint{};
}