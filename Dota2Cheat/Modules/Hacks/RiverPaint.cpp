#include "RiverPaint.h"

void Hacks::RiverPaint::FrameBasedLogic() {
	static auto cvar = CVarSystem::CVars["dota_river_type"];
	cvar.var->value.ui32 = Config::Changer::RiverListIdx;
}
