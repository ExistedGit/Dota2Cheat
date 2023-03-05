#pragma once
#include "../SDK/include.h"

#include "VMT.h"

namespace Hooks {
	// Here we filter entities and put them into their respective collections
	inline CBaseEntity* OnAddEntity(CEntitySystem* thisptr, CBaseEntity* ent, ENT_HANDLE handle) {
		auto className = ent->SchemaBinding()->binaryName;
		if (className) {
			if (strstr(className, "Item_Physical")) {
				ctx.physicalItems.insert(ent);
			}
			else if (!strcmp(className,"CDOTAItemRune")) {
				ctx.runes.insert((CDOTAItemRune*)ent);
			}
			else if (strstr(className, "Unit_Hero")) {
				ctx.heroes.insert(reinterpret_cast<CDOTABaseNPC_Hero*>(ent));
			}
			ctx.entities.insert(ent);
		}

		return VMTs::EntitySystem->GetOriginalMethod<decltype(&OnAddEntity)>(14)(thisptr, ent, handle);
	};

	inline CBaseEntity* OnRemoveEntity(CEntitySystem* thisptr, CBaseEntity* ent, ENT_HANDLE handle) {
		ctx.physicalItems.erase(ent);
		ctx.heroes.erase((CDOTABaseNPC_Hero*)ent);
		ctx.entities.erase(ent);
		ctx.runes.erase((CDOTAItemRune*)ent);

		if (ent == ctx.importantItems.midas)
			ctx.importantItems.midas = nullptr;


		return VMTs::EntitySystem->GetOriginalMethod<decltype(&OnAddEntity)>(15)(thisptr, ent, handle);
	}
}