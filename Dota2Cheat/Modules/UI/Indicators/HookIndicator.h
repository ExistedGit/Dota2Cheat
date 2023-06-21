#pragma once
#include "../../../SDK/pch.h"
#include "../../../CheatSDK/include.h"

namespace Hacks {
	// STUB
	class HookIndicator {
	public:
		Vector CalculateHook(CDOTABaseNPC* enemy);
		void Draw();
	};
}
namespace Modules {
	inline Hacks::HookIndicator HookIndicator{};
}