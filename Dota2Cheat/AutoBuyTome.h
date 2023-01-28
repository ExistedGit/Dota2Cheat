#pragma once
#include "Globals.h"
#include "Config.h"

//extern BaseNpc* assignedHero;
//extern DotaPlayer* localPlayer;
namespace Hacks {

	inline ItemStockInfo* tomeStockInfo;

	inline void AutoBuyTomeInit() {
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
	inline void AutoBuyTomeCheck() {
		if (!Config::AutoBuyTome ||
			tomeStockInfo->ItemID != 0x101) // maybe the tome stock info was not found
			return;

		if (tomeStockInfo->StockCount > 0) 
			localPlayer->BuyItem(0x101);
	}

}