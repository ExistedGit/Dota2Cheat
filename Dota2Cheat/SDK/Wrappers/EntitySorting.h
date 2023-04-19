#pragma once
#include "../Globals/Context.h"

inline void SortEntToCollections(CBaseEntity* ent) {
	std::string_view className = ent->SchemaBinding()->binaryName;

	if (className == "C_DOTA_Item_Physical")
		ctx.physicalItems.insert(ent);
	else if (className.find("Creep") != -1)
		ctx.creeps.insert((CDOTABaseNPC*)ent);
	else if (className == "C_DOTA_Item_Rune")
		ctx.runes.insert((CDOTAItemRune*)ent);
	else if (className.find("C_DOTA_Unit_Hero") != -1
		|| className.find("CDOTA_Unit_Hero") != -1)
		ctx.heroes.insert((CDOTABaseNPC_Hero*)(ent));

	ctx.entities.insert(ent);
}