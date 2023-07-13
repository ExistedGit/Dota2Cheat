#pragma once
#include "../../pch.h"
#include "../../CheatSDK/include.h"

namespace Modules {
	// Restock info is stored inside CDOTA_GameRules
	// It has two of ItemStockInfo for each item(one for each team)
	inline class M_AutoBuyTome {
	private:
		ItemStockInfo* tomeStockInfo;
	public:
		void Reset();
		void Init();
		void OnFrame();
	}  AutoBuyTome{};
}