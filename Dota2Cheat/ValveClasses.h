#pragma once
#include "sdk.h"

class ItemStockInfo : VClass {
private:
	char pad[40]; //idk why tho
public:
	DOTA_GC_TEAM TeamNumber;
	int ItemID;
	float StockDuration, StockTime;
	int StockCount, MaxCount;
	float InitialStockDuration;
	uint32_t PlayerID;
	int BonusDelayedStockCount;
};

class CDOTAGameRules : VClass {
public:
	DOTA_GameState GetGameState() {
		return Member<DOTA_GameState>(Netvars::C_DOTAGamerules::m_nGameState);
	}
	float GetGameTime() {
		return Member<float>(Netvars::C_DOTAGamerules::m_fGameTime);
	}

	DOTA_GameMode GetGameMode() {
		return Member<DOTA_GameMode>(Netvars::C_DOTAGamerules::m_iGameMode);
	}

	std::vector<ItemStockInfo*> GetItemStockInfo() {
		CUtlVector<ItemStockInfo>* arr = (CUtlVector<ItemStockInfo>*)((uintptr_t)this + Netvars::C_DOTAGamerules::m_vecItemStockInfo);
		auto result = std::vector<ItemStockInfo*>{};
		result.reserve(arr->m_Size);
		for (int i = 0; i < arr->m_Size; i++)
			result.push_back(&arr->at(i));
		
		return result;
	}
};


class PlayerResourcePlayerData_t :public VClass {
	char pad[0xc8];
public:
	uint32_t GetPlayerSlot() {
		return Member<uint32_t>(Netvars::PlayerResourcePlayerData_t::m_nPlayerSlot);
	}
};
class C_DOTA_PlayerResource : public VClass {
public:
	CUtlVector<PlayerResourcePlayerData_t> GetVecPlayerTeamData() {
		return Member< CUtlVector<PlayerResourcePlayerData_t>>(Netvars::C_DOTA_PlayerResource::m_vecPlayerTeamData);
	}
};