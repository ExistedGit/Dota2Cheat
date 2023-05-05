#pragma once
#include "../../SDK/pch.h"
#include "../../CheatSDK/include.h"

namespace Hacks {
	class AutoDodge {
	public:
		void FrameBasedLogic();
	};
}
namespace Modules {
	inline Hacks::AutoDodge AutoDodge{};
}