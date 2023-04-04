#pragma once
#include "../../SDK/pch.h"
#include "../../Config.h"

namespace Hacks {
	class AutoMidas {
	public:
		const float usePeriod = 0.1f;
		float lastTime = 0;
		void FrameBasedLogic();
	};
}
namespace Modules {
	inline Hacks::AutoMidas AutoMidas{};
}