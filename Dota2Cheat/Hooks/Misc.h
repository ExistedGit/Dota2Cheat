#pragma once
#include "../SDK/pch.h"

namespace Hooks {
	inline Signatures::BIsEmoticonUnlockedFn oBIsEmoticonUnlocked;

	inline bool hkBIsEmoticonUnlocked(void* thisptr, uint32_t unk) {
		return unk != 0;
	}
}