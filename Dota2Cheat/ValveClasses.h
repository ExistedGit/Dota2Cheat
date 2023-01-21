#pragma once
#include "sdk.h"
#include <vector>

class ItemStockInfo : VClass {
private:
	char pad[40]; //idk why tho
public:
	DOTATeam_t TeamNumber;
	int ItemID;
	float StockDuration, StockTime;
	int StockCount, MaxCount;
	float InitialStockDuration;
	uint32_t PlayerID;
	int BonusDelayedStockCount;
//private:
//	char pad2[0x16]; //idk why tho
};

class CDOTAGameRules : VClass {
public:
	inline GameState GetGameState() {
		return Member<GameState>(0x60);
	}
	inline float GetGameTime() {
		return Member<float>(0x89C);
	}
	inline std::vector<ItemStockInfo*> GetItemStockInfo() {
		auto arr = *(ItemStockInfo**)((uintptr_t)this + 0x730);
		int size = Member<int>(0x728);
		//std::cout << "arr " << arr << " size " << size << '\n';
		auto result = std::vector<ItemStockInfo*>{};
		result.reserve(size);
		for (int i = 0; i < size; i++) {
			//std::cout << std::hex << &arr[i] << '\n';
			result.push_back(&arr[i]);
		}
		return result;
	}
};