#include "AegisAutoPickup.h"

void Hacks::AegisAutoPickup::RemoveIfAegis(CBaseEntity* ent) {
	if (ent == aegis)
		aegis = nullptr;
}

void Hacks::AegisAutoPickup::FrameBasedLogic() {
	if (!aegis) {
		for (auto& physItem : ctx.physicalItems) {
			auto item = Interfaces::EntitySystem->GetEntity(H2IDX(physItem->Member<ENT_HANDLE>(0x990)));
			if (item &&
				strstr(item->GetIdentity()->GetName(), "aegis"))
				aegis = item;
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

	ctx.localPlayer->PrepareOrder(
		DOTA_UNIT_ORDER_PICKUP_ITEM,
		aegis->GetIndex(),
		&Vector::Zero,
		0,
		DOTA_ORDER_ISSUER_PASSED_UNIT_ONLY,
		ctx.assignedHero,
		false,
		true);
	return;
}
