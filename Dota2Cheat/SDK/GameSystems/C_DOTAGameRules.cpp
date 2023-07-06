#include "C_DOTAGameRules.h"
#include "../Globals/GameSystems.h"
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

std::vector<ItemStockInfo*> CDOTAGameRules::GetItemStockInfo() {
	auto arr = MemberInline<CUtlVector<ItemStockInfo>>(Netvars::C_DOTAGamerules::m_vecItemStockInfo);
	auto result = std::vector<ItemStockInfo*>{};
	result.reserve(arr->m_Size);
	for (int i = 0; i < arr->m_Size; i++)
		result.push_back(&arr->at(i));

	return result;
}
