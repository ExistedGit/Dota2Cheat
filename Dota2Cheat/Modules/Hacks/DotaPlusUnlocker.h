#pragma once
#include "../../pch.h"
#include "../../CheatSDK/Config.h"

namespace Modules {
	inline class DotaPlusUnlocker {
		bool updateQueued = true; // will update when injected
	public:
		// rebuilt from xref: "Failed to find CDOTAGameAccountPlus"
		void UpdateDotaPlusStatus();

		void QueueUpdate() {
			updateQueued = true;
		}
	} DotaPlusUnlocker;
}