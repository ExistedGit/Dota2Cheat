#include "RiverPaint.h"

void Hacks::RiverPaint::FrameBasedLogic() {
	static auto cvar = CVarSystem::CVar["dota_river_type"];
	cvar.var->value.ui32 = Config::RiverListIdx;
}
