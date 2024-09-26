#include "AutoBuyTome.h"

void Modules::M_AutoBuyTome::Reset() {
	tomeStockInfo = nullptr;
}

void Modules::M_AutoBuyTome::Init() {
	if (!CGameRules::Get() ||
		!ctx.localHero)
		return;

	auto vec = CGameRules::Get()->GetItemStockInfo();
	for (auto& info : vec) {
		if (info.iTeamNumber == ctx.localHero->GetTeam() &&
			info.nItemAbilityID == 0x101) { // tome's id is 257
			tomeStockInfo = &info;
			LogI("Found Tome of Knowledge stock info");
			break;
		}
	}
}

void Modules::M_AutoBuyTome::OnFrame() {
	if (// !Config::AutoBuyTome ||
		tomeStockInfo->nItemAbilityID != 0x101) // maybe the tome stock info was not found
		return;

	if (tomeStockInfo->iStockCount > 0)
		ctx.localPlayer->BuyItem(0x101);
}
