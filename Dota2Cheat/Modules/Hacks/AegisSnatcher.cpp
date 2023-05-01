#include "AegisSnatcher.h"

void Hacks::AegisSnatcher::RemoveIfAegis(CBaseEntity* ent) {
	if (ent == aegis)
		aegis = nullptr;
}

void Hacks::AegisSnatcher::FrameBasedLogic() {

	if (!aegis) {
		for (auto& physItem : ctx.physicalItems) {
			auto item = Interfaces::EntitySystem->GetEntity(H2IDX(physItem->Member<ENT_HANDLE>(Netvars::C_DOTA_Item_Physical::m_hItem)));
			if (item &&
				strstr(item->GetIdentity()->GetName(), "aegis"))
				aegis = physItem;
		}
		if (!aegis)
			return;
	}

	bool slotsFull = true;
	auto items = ctx.assignedHero->GetInventory()->GetItems();
	for (int i = 0; i < 6; ++i)
		if (!HVALID(items[i])) {
			slotsFull = false;
			break;
		}

	//TODO: implement a slot switcher for items
	if (slotsFull)
		return;

	if (!IsWithinRadius(aegis->GetPos(), ctx.assignedHero->GetPos(), 130))
		return;

	auto gameTime = GameSystems::GameRules->GetGameTime();
	if (gameTime - lastPickupTime >= 0.2f) {

		ctx.localPlayer->PrepareOrder(
			DOTA_UNIT_ORDER_PICKUP_ITEM,
			aegis->GetIndex(),
			&Vector::Zero,
			0,
			DOTA_ORDER_ISSUER_PASSED_UNIT_ONLY,
			ctx.assignedHero,
			false,
			true);

		lastPickupTime = gameTime;
	}

	return;
}
