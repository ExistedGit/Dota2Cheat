#include "EntitySystemEvents.h"
#include "../Lua/LuaModules.h"
// Here we filter entities and put them into their respective collections
CBaseEntity* Hooks::hkOnAddEntity(CEntitySystem* thisptr, CBaseEntity* ent, ENT_HANDLE handle) {
	if (ctx.gameStage == Context::GameStage::IN_MATCH) {
		auto className = ent->SchemaBinding()->binaryName;
		if (className) {
			if (strstr(className, "Item_Physical")) {
				ctx.physicalItems.insert(ent);
			}
			else if (!strcmp(className, "C_DOTA_Item_Rune")) {
				ctx.runes.insert((CDOTAItemRune*)ent);
			}
			else if (strstr(className, "Unit_Hero")) {
				ctx.heroes.insert(reinterpret_cast<CDOTABaseNPC_Hero*>(ent));
			}
			ctx.entities.insert(ent);
		}
		Lua::CallModuleFunc("OnAddEntity", ent);
	}
	return oOnAddEntity(thisptr, ent, handle);
}

CBaseEntity* Hooks::hkOnRemoveEntity(CEntitySystem* thisptr, CBaseEntity* ent, ENT_HANDLE handle) {
	if (ctx.gameStage == Context::GameStage::IN_MATCH) {
		ctx.physicalItems.erase(ent);
		ctx.heroes.erase((CDOTABaseNPC_Hero*)ent);
		ctx.entities.erase(ent);
		ctx.runes.erase((CDOTAItemRune*)ent);

		if (ent == ctx.importantItems.midas)
			ctx.importantItems.midas = nullptr;

		Lua::CallModuleFunc("OnRemoveEntity", ent);
	}
	return oOnRemoveEntity(thisptr, ent, handle);
}
