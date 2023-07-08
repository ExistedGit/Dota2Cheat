#pragma once

#include "../Protobufs/dota_shared_enums.pb.h"
#include "../Base/VClass.h"
#include "../Base/Definitions.h"
#include "../Base/CUtlVector.h"
#include "../Netvars.h"

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
public:
	GETTER(DOTA_GameState, GetGameState, Netvars::C_DOTAGamerules::m_nGameState);
	GETTER(DOTA_GameMode, GetGameMode, Netvars::C_DOTAGamerules::m_iGameMode);
	GETTER(bool, IsGamePaused, Netvars::C_DOTAGamerules::m_bGamePaused);
	GETTER(uint64_t, GetMatchID, Netvars::C_DOTAGamerules::m_unMatchID64);
	GETTER(int32_t, GetRiverType, Netvars::C_DOTAGamerules::m_nRiverType);
	GETTER(float, GetPreGameStartTime, Netvars::C_DOTAGamerules::m_flPreGameStartTime);
	GETTER(float, GetGameStartTime, Netvars::C_DOTAGamerules::m_flGameStartTime);
	GETTER(float, GetGameLoadTime, Netvars::C_DOTAGamerules::m_flGameLoadTime);
	GETTER(float, GetGameEndTime, Netvars::C_DOTAGamerules::m_flGameEndTime);

	float GetGameTime();

	GETTER(CUtlVector<ItemStockInfo>, GetItemStockInfo, Netvars::C_DOTAGamerules::m_vecItemStockInfo);
};