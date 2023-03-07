#pragma once
#include "../Protobufs/dota_shared_enums.pb.h"
#include "../Base/VClass.h"
#include "../Netvars.h"
#include "../Base/CUtlVector.h"

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

class CDOTAGameRules : public VClass {
public:
	DOTA_GameState GetGameState() {
		return Member<DOTA_GameState>(Netvars::C_DOTAGamerules::m_nGameState);
	}
	float GetGameTime() {
		return Member<float>(0);
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