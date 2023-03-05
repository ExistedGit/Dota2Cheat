#pragma once
#include "../../SDK/include.h"
#include "../../Config.h"

namespace Hacks {
	// Restock info is stored inside CDOTA_GameRules
	// It has two of ItemStockInfo for each item(one for each team)
	class AutoBuyTome {
	private:
		ItemStockInfo* tomeStockInfo;
	public:
		void Reset() {
			tomeStockInfo = nullptr;
		}
		void Init() {
			if (!GameSystems::GameRules ||
				!ctx.assignedHero)
				return;

			auto vec = GameSystems::GameRules->GetItemStockInfo();
			for (auto& info : vec) {
				if (info->TeamNumber == ctx.assignedHero->GetTeam() &&
					info->ItemID == 0x101) { // tome's id is 257
					tomeStockInfo = info;
					std::cout << "FOUND TOME STOCK INFO\n";
					break;
				}
			}
		}
		void FrameBasedLogic() {
			if (!Config::AutoBuyTome ||
				tomeStockInfo->ItemID != 0x101) // maybe the tome stock info was not found
				return;

			if (tomeStockInfo->StockCount > 0)
				ctx.localPlayer->BuyItem(0x101);
		}
	};

}

namespace Modules {
	inline Hacks::AutoBuyTome AutoBuyTome{};
}