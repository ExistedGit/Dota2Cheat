#pragma once
#include "../../SDK/pch.h"
#include "../../Config.h"

namespace Hacks {
	// Restock info is stored inside CDOTA_GameRules
	// It has two of ItemStockInfo for each item(one for each team)
	class AutoBuyTome {
	private:
		ItemStockInfo* tomeStockInfo;
	public:
		void Reset();
		void Init();
		void FrameBasedLogic();
	};

}

namespace Modules {
	inline Hacks::AutoBuyTome AutoBuyTome{};
}