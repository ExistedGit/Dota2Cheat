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
		return Member<DOTA_GameState>(0x60);
	}
	float GetGameTime() {
		return Member<float>(0x89C);
	}

	DOTA_GameMode GetGameMode() {
		return Member<DOTA_GameMode>(0xcc);
	}

	std::vector<ItemStockInfo*> GetItemStockInfo() {
		CUtlVector<ItemStockInfo>* arr = (CUtlVector<ItemStockInfo>*)((uintptr_t)this + 0x728);
		auto result = std::vector<ItemStockInfo*>{};
		result.reserve(arr->m_Size);
		for (int i = 0; i < arr->m_Size; i++)
			result.push_back(&arr->at(i));
		
		return result;
	}
};


class PlayerResourcePlayerData_t :public VClass {
	char pad[0xd0 - 0x8];
public:
	uint32_t GetPlayerSlot() {
		return Member<uint32_t>(0xc8);
	}
};
class CDOTAPlayerResource : public VClass {
public:
	CUtlVector<PlayerResourcePlayerData_t> GetVecTeamPlayerData() {
		return Member< CUtlVector<PlayerResourcePlayerData_t>>(0x5d0);
	}
};