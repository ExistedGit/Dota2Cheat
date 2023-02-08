#pragma once
#include "Globals.h"
#include "Config.h"

namespace Hacks {
	class AutoBuyTome {
	private:
		ItemStockInfo* tomeStockInfo;
	public:
		inline void Reset() {
			tomeStockInfo = nullptr;
		}
		inline void Init() {
			if (Globals::GameRules == nullptr ||
				assignedHero == nullptr)
				return;

			auto vec = Globals::GameRules->GetItemStockInfo();
			for (auto& info : vec) {
				if (info->TeamNumber == assignedHero->GetTeam() &&
					info->ItemID == 0x101) { // tome's id is 257
					tomeStockInfo = info;
					std::cout << "FOUND TOME STOCK INFO\n";
					break;
				}
			}
		}
		inline void FrameBasedLogic() {
			if (!Config::AutoBuyTome ||
				tomeStockInfo->ItemID != 0x101) // maybe the tome stock info was not found
				return;

			if (tomeStockInfo->StockCount > 0)
				localPlayer->BuyItem(0x101);
		}
	};

}

namespace Modules {
	Hacks::AutoBuyTome AutoBuyTome{};
}