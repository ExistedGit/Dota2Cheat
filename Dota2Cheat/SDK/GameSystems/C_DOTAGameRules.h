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
	typedef void(*GetGameTimeFn)(float* out, bool unk);
	inline static GetGameTimeFn GetGameTimeFunc{};

	GETTER(DOTA_GameState, GetGameState, Netvars::C_DOTAGamerules::m_nGameState);
	GETTER(DOTA_GameMode, GetGameMode, Netvars::C_DOTAGamerules::m_iGameMode);
	GETTER(bool, IsGamePaused, Netvars::C_DOTAGamerules::m_bGamePaused);

	float GetGameTime();
	std::vector<ItemStockInfo*> GetItemStockInfo();

	static void BindLua(sol::state& lua);
};