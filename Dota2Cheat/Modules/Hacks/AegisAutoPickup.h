#pragma once
#include "../../SDK/include.h"

namespace Hacks {
	class AegisAutoPickup {
	public:
		bool PickUpIfAegis(CBaseEntity* ent) {
			auto className = ent->SchemaBinding()->binaryName;
			if (!strstr(className, "Item_Physical"))
				return false;
			auto item = Interfaces::EntitySystem->GetEntity(H2IDX(ent->Member<ENT_HANDLE>(0x990)));
			if (!strstr(item->GetIdentity()->GetName(), "aegis"))
				return false;

			if (IsWithinRadius(ent->GetPos(), ctx.assignedHero->GetPos(), 130)) {
				ctx.localPlayer->PrepareOrder(DOTA_UNIT_ORDER_PICKUP_ITEM, ent->GetIndex(), &Vector::Zero, 0, DOTA_ORDER_ISSUER_PASSED_UNIT_ONLY, ctx.assignedHero, false, false);
				return true;
			}
			
			return false;
		}
	};
}
namespace Modules {
	inline Hacks::AegisAutoPickup AegisAutoPickup{};
}