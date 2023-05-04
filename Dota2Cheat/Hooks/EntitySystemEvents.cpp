#include "EntitySystemEvents.h"

// Here we filter entities and put them into their respective collections
CBaseEntity* Hooks::hkOnAddEntity(CEntitySystem* thisptr, CBaseEntity* ent, ENT_HANDLE handle) {
	SortEntToCollections(ent);
	return oOnAddEntity(thisptr, ent, handle);
}

CBaseEntity* Hooks::hkOnRemoveEntity(CEntitySystem* thisptr, CBaseEntity* ent, ENT_HANDLE handle) {
	if (ent->SchemaBinding()->binaryName) {
		ctx.physicalItems.erase(ent);
		ctx.heroes.erase((CDOTABaseNPC_Hero*)ent);
		ctx.creeps.erase((CDOTABaseNPC*)ent);
		ctx.entities.erase(ent);
		ctx.runes.erase((CDOTAItemRune*)ent);

		for (auto it = HeroData[ctx.localHero].Items.begin(); it != HeroData[ctx.localHero].Items.end(); ++it)
			if (it->second == ent) {
				it = HeroData[ctx.localHero].Items.erase(it);
				break;
			}

		Modules::AegisSnatcher.RemoveIfAegis(ent);
	}
	return oOnRemoveEntity(thisptr, ent, handle);
}
