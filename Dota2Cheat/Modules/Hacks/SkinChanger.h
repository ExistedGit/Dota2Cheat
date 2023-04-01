#pragma once
#include "../SDK/pch.h"
#include <queue>

// please for the love of god don't interpret this as a complete module
// we still have a long way to go!
namespace Hacks {
	class SkinChanger {
	public:
		bool AddItem(uint32_t unDefIndex);
	};
}
namespace Modules {
	inline Hacks::SkinChanger SkinChanger;
}