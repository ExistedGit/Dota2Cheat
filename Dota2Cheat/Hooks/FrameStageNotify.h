#pragma once
#include "../pch.h"
#include "../CheatSDK/include.h"

// Source engine's networking relies on executing frames in stages, such as
// receiving net packages, simulating, predicting and only then rendering
// There's no good public version of ClientFrameStage_t(the enum of stages)
// For now I won't go as far as to analyze what each of them does and give them names

namespace Hooks {
	inline void* oFrameStageNotify{};
	void hkFrameStageNotify(void* thisptr, int stage);
}