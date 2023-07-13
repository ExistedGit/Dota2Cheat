#pragma once
#include "../pch.h"
#include "../Utils/Drawing.h"
#include "../CheatSDK/include.h"

namespace Modules {
	inline class M_LastHitMarker {
	public:
		void DrawCircleFor(CDOTABaseNPC* creep);
		void Draw();
	} LastHitMarker{};
}