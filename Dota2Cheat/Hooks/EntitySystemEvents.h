#pragma once
#include "../Wrappers.h"

#include "VMT.h"
#include "../SunStrikeHighlighter.h"

namespace Hooks {
	inline BaseEntity* OnAddEntity(CEntitySystem* thisptr, BaseEntity* ent, ENT_HANDLE handle) {
		auto className = ent->SchemaBinding()->binaryName;
		if (className) {
			if (TestStringFilters(className, { "Item_Physical" })) {
				ctx.physicalItems.insert(ent);
			}
			else if (TestStringFilters(className, { "BaseNPC" })) {
				const char* idName = ent->GetIdentity()->GetName();
				if (Modules::SunStrikeHighlighter.SunStrikeIncoming && !idName)
					Modules::SunStrikeHighlighter.QueueThinker(ent);
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

		return VMTs::Entity->GetOriginalMethod<decltype(&OnAddEntity)>(15)(thisptr, ent, handle);
	}
}