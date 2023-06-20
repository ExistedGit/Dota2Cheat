#include "C_DOTAGameRules.h"
#include "../Globals/GameSystems.h"
#include "CGlobalVars.h"

// Suggested by og, reversed by Morphling and rewritten by ExistedDim4
float CDOTAGameRules::GetGameTime() {
	auto gpGlobals = CGlobalVars::GetInstance();
	auto someCondition = !gpGlobals->Member<bool>(61) && !gpGlobals->Member<bool>(60);

	if (someCondition)
	{
		auto func = gpGlobals->Member<Function>(32);
		if (func)
			func(1i64, this ? gpGlobals : nullptr);
		if (!this)
			return gpGlobals->Member<float>(44);
	}

	float tickToSeconds = gpGlobals->Member<float>(68);
	auto totalPausedTicks = Member<uint32_t>(Netvars::C_DOTAGamerules::m_nTotalPausedTicks);

	if (IsGamePaused())
		return (Member<int>(Netvars::C_DOTAGamerules::m_nPauseStartTick) - totalPausedTicks) * tickToSeconds;
	return gpGlobals->Member<float>(44) - totalPausedTicks * tickToSeconds;
}

std::vector<ItemStockInfo*> CDOTAGameRules::GetItemStockInfo() {
	CUtlVector<ItemStockInfo>* arr = (CUtlVector<ItemStockInfo>*)((uintptr_t)this + Netvars::C_DOTAGamerules::m_vecItemStockInfo);
	auto result = std::vector<ItemStockInfo*>{};
	result.reserve(arr->m_Size);
	for (int i = 0; i < arr->m_Size; i++)
		result.push_back(&arr->at(i));

	return result;
}

void CDOTAGameRules::BindLua(sol::state& lua) {
	auto type = lua.new_usertype<CDOTAGameRules>("CDOTAGameRules");

	type["GetGameState"] = &CDOTAGameRules::GetGameState;
	type["GetGameMode"] = &CDOTAGameRules::GetGameMode;
	type["IsGamePaused"] = &CDOTAGameRules::IsGamePaused;
	type["GetGameTime"] = &CDOTAGameRules::GetGameTime;
	type["GetItemStockInfo"] = &CDOTAGameRules::GetItemStockInfo;
	type["GetMatchID"] = &CDOTAGameRules::GetMatchID;
	type["GetPreGameStartTime"] = &CDOTAGameRules::GetPreGameStartTime;
	type["GetGameStartTime"] = &CDOTAGameRules::GetGameStartTime;
	type["GetGameLoadTime"] = &CDOTAGameRules::GetGameLoadTime;
	type["GetGameEndTime"] = &CDOTAGameRules::GetGameEndTime;

}
