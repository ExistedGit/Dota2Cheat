#pragma once
#include "../../SDK/pch.h"
#include "../../CheatSDK/Config.h"

namespace Hacks {
	class DotaPlusUnlocker {
		bool plusStatus = false;
	public:
		// rebuilt from xref: "Failed to find CDOTAGameAccountPlus"
		void UpdateDotaPlusStatus();
	};
}
namespace Modules {
	inline Hacks::DotaPlusUnlocker DotaPlusUnlocker{};
}