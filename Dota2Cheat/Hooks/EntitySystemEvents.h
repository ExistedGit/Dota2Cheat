#pragma once
#include "../Wrappers.h"

#include "VMT.h"

namespace Hooks {
	inline BaseEntity* OnAddEntity(CEntitySystem* thisptr, BaseEntity* ent, ENT_HANDLE handle) {
		auto className = ent->SchemaBinding()->binaryName;
		if (className) {
			if (strstr(className, "Item_Physical")) {
				ctx.physicalItems.insert(ent);
			}
			else if (strstr(className, "Unit_Hero")) {
				ctx.heroes.insert(reinterpret_cast<BaseNpc*>(ent));
			}
			ctx.entities.insert(ent);
		}

		return VMTs::Entity->GetOriginalMethod<decltype(&OnAddEntity)>(14)(thisptr, ent, handle);
	};

	inline BaseEntity* OnRemoveEntity(CEntitySystem* thisptr, BaseEntity* ent, ENT_HANDLE handle) {
		ctx.physicalItems.erase(ent);
		ctx.heroes.erase((BaseNpc*)ent);
		ctx.entities.erase(ent);

		if (ent == ctx.importantItems.midas)
			ctx.importantItems.midas = nullptr;


		return VMTs::Entity->GetOriginalMethod<decltype(&OnAddEntity)>(15)(thisptr, ent, handle);
	}
}