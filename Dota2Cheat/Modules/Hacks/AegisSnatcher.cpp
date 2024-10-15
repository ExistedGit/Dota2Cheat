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

	auto items = ctx.localHero->GetItems();

	// If there are no empty slots
	if (std::find_if(items.begin(), items.end(), [](auto x) { return !x.IsValid(); }) == items.end())
		return;

	if (!IsWithinRadius(aegis->GetPos(), ctx.localHero->GetPos(), 130))
		return;

	auto gameTime = CGameRules::Get()->GetGameTime();
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

void Modules::AegisSnatcher::OnEntityAdded(const EntityWrapper& ent)
{
}
