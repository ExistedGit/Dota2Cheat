#include "EntityList.h"

void CEntityList::OnEntityCreated(CBaseEntity* ent) {
	std::lock_guard<std::mutex> lock(mEntities);
	if (!ent->SchemaBinding()->binaryName || Entities.contains(ent->GetIndex()))
		return;

	std::string_view className = ent->SchemaBinding()->binaryName;
	if (className == "C_DOTAGamerulesProxy")
		GameSystems::GameRules = ent->Member<CDOTAGameRules*>(Netvars::C_DOTAGamerulesProxy::m_pGameRules);


	using enum EntityType;
	EntityType entType = Undefined;

	if (className.starts_with("C_DOTA_Unit_Hero")
		|| className.starts_with("CDOTA_Unit_Hero")) {
		LogF(LP_NONE, "Hero: {}", (void*)ent);
		// Sleep(15000);
		entType = Hero;
	}
	else if (className.find("Creep") != -1)
		entType = Creep;

	if (entType == Undefined) {
		if (className == "C_DOTA_Item_Rune") entType = Rune;
		else if (className == "C_DOTA_Item_Physical") entType = PhysicalItem;
	}

	// Entity could not be classified
	if (entType == Undefined)
		return;

	EntityWrapper wrap{
		.ent = ent,
		.type = entType
	};

	if (wrap.type == Creep) {
		if (className == "C_DOTA_BaseNPC_Creep_Siege")
			wrap.creepType = CreepType::Siege;
		else if (className == "C_DOTA_BaseNPC_Creep_Lane") {
			if (((CDOTABaseNPC*)ent)->GetAttackCapabilities() == DOTA_UNIT_CAP_MELEE_ATTACK)
				wrap.creepType = CreepType::LaneMelee;
			else
				wrap.creepType = CreepType::LaneRanged;
		}
	}
	Entities[ent->GetIndex()] = wrap;
	DispatchEntityAdded(wrap);
}

void CEntityList::OnEntityDeleted(CBaseEntity* ent) {
	std::lock_guard<std::mutex> lock(mEntities);
	if (Entities.contains(ent->GetIndex())) {
		DispatchEntityRemoved(Entities[ent->GetIndex()]);
		Entities.erase(ent->GetIndex());
	}
}