#pragma once
#include "../../SDK/pch.h"
#include "../../CheatSDK/Config.h"

namespace Modules {
inline 
	class DotaPlusUnlocker {
		bool plusStatus = false;
	public:
		// rebuilt from xref: "Failed to find CDOTAGameAccountPlus"
		void UpdateDotaPlusStatus();
	} DotaPlusUnlocker;
}