#pragma once
#include "../Entities/CDOTABaseNPC.h"

enum class CreepType {
	LaneMelee,
	LaneRanged,
	Siege
};

class CreepWrapper {
public:
	CDOTABaseNPC* ent = nullptr;
	CreepType creepType;

	CreepWrapper() {}
	CreepWrapper(CDOTABaseNPC* ent) : ent(ent) {
		if (!ent->SchemaBinding()->binaryName)
			return;

		std::string_view name = ent->SchemaBinding()->binaryName;
		if (name == "C_DOTA_BaseNPC_Creep_Siege")
			creepType = CreepType::Siege;
		else if (name == "C_DOTA_BaseNPC_Creep_Lane") {
			if (ent->GetAttackCapabilities() == DOTA_UNIT_CAP_MELEE_ATTACK)
				creepType = CreepType::LaneMelee;
			else
				creepType = CreepType::LaneRanged;
		}
	}
};

// Comparator for std::set
struct CreepWrapper_less {
	bool operator()(const CreepWrapper& w1, const CreepWrapper& w2)const {
		return w1.ent < w2.ent;
	}
};