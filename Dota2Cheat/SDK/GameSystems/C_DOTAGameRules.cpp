#include "C_DOTAGameRules.h"
#include "CGlobalVars.h"

// Suggested by og, reversed by Morphling and rewritten by ExistedDim4
float CDOTAGameRules::GetGameTime() {
	auto gpGlobals = CGlobalVars::GetInstance();
	float tickToSeconds = gpGlobals->Member<float>(68);
	auto totalPausedTicks = Member<uint32_t>(Netvars::C_DOTAGamerules::m_nTotalPausedTicks);

	if (IsGamePaused())
		return (Member<int>(Netvars::C_DOTAGamerules::m_nPauseStartTick) - totalPausedTicks) * tickToSeconds;
	return gpGlobals->Member<float>(44) - totalPausedTicks * tickToSeconds;
}

