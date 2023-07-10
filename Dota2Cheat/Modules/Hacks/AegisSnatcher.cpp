#include "AegisSnatcher.h"

void Modules::AegisSnatcher::OnEntityRemoved(const EntityWrapper& wrap) {
	if (wrap.ent == aegis)
		aegis = nullptr;
}

void Modules::AegisSnatcher::OnFrame() {
	if (!aegis) {
		auto findAegis = [this](auto& physItem) {
			auto item = physItem->Member<CHandle<>>(Netvars::C_DOTA_Item_Physical::m_hItem);
			if (item &&
				strstr(item->GetIdentity()->GetName(), "aegis"))
				aegis = physItem;
		};

		EntityList.ForEachOfType(EntityType::PhysicalItem, findAegis);

		if (!aegis)
			return;
	}

	bool hasEmptySlot = false;
	auto items = ctx.localHero->GetInventory()->GetItems();
	for (int i = 0; i < 6; ++i)
		if (!HVALID(items[i])) {
			hasEmptySlot = true;
			break;
		}

	if (!hasEmptySlot)
		return;

	if (!IsWithinRadius(aegis->GetPos(), ctx.localHero->GetPos(), 130))
		return;

	auto gameTime = GameSystems::GameRules->GetGameTime();
	if (gameTime - lastPickupTime >= 0.2f) {

		ctx.localPlayer->PrepareOrder(
			Order().SetType(DOTA_UNIT_ORDER_PICKUP_ITEM)
			.SetTargetIndex(aegis->GetIndex())
			.SetIssuer(ctx.localHero)
		);

		lastPickupTime = gameTime;
	}

	return;
}
