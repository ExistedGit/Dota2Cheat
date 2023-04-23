#pragma once
#include "../SDK/pch.h"
#include "../Utils/Drawing.h"
#include "../CheatSDK/include.h"

namespace Hacks {
	class LastHitMarker {
	public:
		void DrawCircleFor(CDOTABaseNPC* creep);
		void Draw();
	};
}

namespace Modules {
	inline Hacks::LastHitMarker LastHitMarker{};
}