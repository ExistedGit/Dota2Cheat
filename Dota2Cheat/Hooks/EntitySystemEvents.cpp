#include "EntitySystemEvents.h"
#include "../Lua/LuaModules.h"
#include "../Modules/Hacks/AegisAutoPickup.h"

// Here we filter entities and put them into their respective collections
CBaseEntity* Hooks::hkOnAddEntity(CEntitySystem* thisptr, CBaseEntity* ent, ENT_HANDLE handle) {
	if (ctx.gameStage == Context::GameStage::IN_GAME
		&& ent->SchemaBinding()->binaryName) {
		std::string_view className = ent->SchemaBinding()->binaryName;

		if (className == "C_DOTA_Item_Physical")
			ctx.physicalItems.insert(ent);
		else if (className.find("Creep") != -1)
			ctx.creeps.insert((CDOTABaseNPC*)ent);
		else if (className == "C_DOTA_Item_Rune")
			ctx.runes.insert((CDOTAItemRune*)ent);
		else if (className.find("C_DOTA_Unit_Hero") != -1)
			ctx.heroes.insert(reinterpret_cast<CDOTABaseNPC_Hero*>(ent));

		ctx.entities.insert(ent);
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

	if (ent == ctx.importantItems.midas)
		ctx.importantItems.midas = nullptr;

	Modules::AegisAutoPickup.RemoveIfAegis(ent);

	if (ctx.gameStage == Context::GameStage::IN_GAME)
		Lua::CallModuleFunc("OnRemoveEntity", ent);

	return oOnRemoveEntity(thisptr, ent, handle);
}
