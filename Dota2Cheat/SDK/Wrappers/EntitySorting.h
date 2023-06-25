#pragma once
#include "../Globals/Context.h"
#include <consthash/cityhash32.hxx>
#include <cityhash/city.h>

inline void SortEntToCollections(CBaseEntity* ent) {
	if (!ent->SchemaBinding()
		|| !ent->SchemaBinding()->binaryName)
		return;

	std::string_view className = ent->SchemaBinding()->binaryName;
	
	switch (CityHash32(className)) {
	case "C_DOTA_Item_Physical"_city32: ctx.physicalItems.insert(ent); break;
	case "C_DOTA_Item_Rune"_city32: ctx.runes.insert((CDOTAItemRune*)ent); break;
	}

	if (className.starts_with("C_DOTA_Unit_Hero")
		|| className.starts_with("CDOTA_Unit_Hero"))
		ctx.heroes.insert((CDOTABaseNPC_Hero*)ent);
	else if (className.find("Creep") != -1)
		ctx.creeps.insert((CDOTABaseNPC*)ent);

	ctx.entities.insert(ent);
}