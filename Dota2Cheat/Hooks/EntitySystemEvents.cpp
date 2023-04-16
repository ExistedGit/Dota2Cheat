#include "EntitySystemEvents.h"

// Here we filter entities and put them into their respective collections
CBaseEntity* Hooks::hkOnAddEntity(CEntitySystem* thisptr, CBaseEntity* ent, ENT_HANDLE handle) {
	if (ctx.gameStage == Context::GameStage::IN_GAME
		&& ent->SchemaBinding()->binaryName) {

		SortEntToCollections(ent);
		Lua::CallModuleFunc("OnAddEntity", ent);
	}
	return oOnAddEntity(thisptr, ent, handle);
}

CBaseEntity* Hooks::hkOnRemoveEntity(CEntitySystem* thisptr, CBaseEntity* ent, ENT_HANDLE handle) {

	ctx.physicalItems.erase(ent);
	ctx.heroes.erase((CDOTABaseNPC_Hero*)ent);
	ctx.creeps.erase((CDOTABaseNPC*)ent);
	ctx.entities.erase(ent);
	ctx.runes.erase((CDOTAItemRune*)ent);

	for (auto it = ctx.ImportantItems.begin(); it != ctx.ImportantItems.end(); ++it)
		if (it->second == ent) {
			it = ctx.ImportantItems.erase(it);
			break;
		}

	Modules::AegisAutoPickup.RemoveIfAegis(ent);

	if (ctx.gameStage == Context::GameStage::IN_GAME)
		Lua::CallModuleFunc("OnRemoveEntity", ent);

	return oOnRemoveEntity(thisptr, ent, handle);
}
