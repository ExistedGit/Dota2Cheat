#include "AutoBuyTome.h"

void Hacks::AutoBuyTome::Reset() {
	tomeStockInfo = nullptr;
}

void Hacks::AutoBuyTome::Init() {
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

void Hacks::AutoBuyTome::FrameBasedLogic() {
	if (!Config::AutoBuyTome ||
		tomeStockInfo->ItemID != 0x101) // maybe the tome stock info was not found
		return;

	if (tomeStockInfo->StockCount > 0)
		ctx.localPlayer->BuyItem(0x101);
}
