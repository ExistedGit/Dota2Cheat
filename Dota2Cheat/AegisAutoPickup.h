#pragma once
#include "Wrappers.h"

namespace Hacks {
	class AegisAutoPickup {
	public:
		void PickUpIfAegis(BaseEntity* ent) {
			auto className = ent->SchemaBinding()->binaryName;
			if (!strstr(className, "Item_Physical"))
				return;
			auto item = Interfaces::EntitySystem->GetEntity(H2IDX(ent->Member<ENT_HANDLE>(0x990)));
			if (!strstr(item->GetIdentity()->GetName(), "aegis"))
				return;

			if (IsWithinRadius(ent->GetPos2D(), assignedHero->GetPos2D(), 130))
				localPlayer->PrepareOrder(DOTA_UNIT_ORDER_PICKUP_ITEM, H2IDX(ent->GetIdentity()->entHandle), &Vector3::Zero, 0, DOTA_ORDER_ISSUER_PASSED_UNIT_ONLY, assignedHero, false, false);
			
		}
	};
}
namespace Modules {
	inline Hacks::AegisAutoPickup AegisAutoPickup{};
}