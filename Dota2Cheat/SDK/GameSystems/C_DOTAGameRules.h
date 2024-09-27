#pragma once

#include <dota_shared_enums.pb.h>
#include "../Base/VClass.h"
#include "../Base/Definitions.h"
#include "../Base/CUtlVector.h"
#include "../Netvars.h"

#include "CGlobalVars.h"

class ItemStockInfo : VClass {
private:
	char pad[0x28];
public:
	DOTA_GC_TEAM iTeamNumber;
	int nItemAbilityID;
	float fStockDuration, fStockTime;
	int iStockCount, iMaxCount;
	float fInitialStockDuration;
	uint32_t iPlayerID;
	int iBonusDelayedStockCount;
};

class CDOTAGameRules : public VClass {
	static inline CDOTAGameRules* inst = nullptr; 
public:
	GETTER(DOTA_GameState, GetGameState, Netvars::C_DOTAGamerules::m_nGameState);
	GETTER(DOTA_GameMode, GetGameMode, Netvars::C_DOTAGamerules::m_iGameMode);
	GETTER(bool, IsGamePaused, Netvars::C_GameRules::m_bGamePaused);
	GETTER(uint64_t, GetMatchID, Netvars::C_DOTAGamerules::m_unMatchID64);
	GETTER(int32_t, GetRiverType, Netvars::C_DOTAGamerules::m_nRiverType);
	GETTER(float, GetPreGameStartTime, Netvars::C_DOTAGamerules::m_flPreGameStartTime);
	GETTER(float, GetGameStartTime, Netvars::C_DOTAGamerules::m_flGameStartTime);
	GETTER(float, GetGameLoadTime, Netvars::C_DOTAGamerules::m_flGameLoadTime);
	GETTER(float, GetGameEndTime, Netvars::C_DOTAGamerules::m_flGameEndTime);
	GETTER(CUtlVector<ItemStockInfo>, GetItemStockInfo, Netvars::C_DOTAGamerules::m_vecItemStockInfo);

	// Suggested by og, reversed by Morphling and rewritten by ExistedDim4
	float GetGameTime() const {
		auto gpGlobals = CGlobalVars::Get();
		float tickToSeconds = gpGlobals->Member<float>(68);
		auto totalPausedTicks = Member<uint32_t>(Netvars::C_GameRules::m_nTotalPausedTicks);

		if (IsGamePaused())
			return (Member<int>(Netvars::C_GameRules::m_nPauseStartTick) - totalPausedTicks) * tickToSeconds;
		return gpGlobals->Member<float>(44) - totalPausedTicks * tickToSeconds;
	}

	// Instance initialization is deferred to game start
	static void Set(CDOTAGameRules* inst) {
		CDOTAGameRules::inst = inst;
	}

	static CDOTAGameRules* Get() {
		return inst;
	}
};

using CGameRules = CDOTAGameRules;