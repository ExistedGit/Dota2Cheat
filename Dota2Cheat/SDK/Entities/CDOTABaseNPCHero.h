#pragma once
#include "CDOTABaseNPC.h"

class CDOTABaseNPC_Hero : public CDOTABaseNPC {
public:
	struct HeroAttributes {
		float strength, agility, intellect;
	};

	HeroAttributes GetAttributes() {
		return Member<HeroAttributes>(Netvars::C_DOTA_BaseNPC_Hero::m_flStrengthTotal);
	}

	bool IsIllusion() {
		return HVALID(Member<ENT_HANDLE>(Netvars::C_DOTA_BaseNPC_Hero::m_hReplicatingOtherHeroModel));
	}

	static void BindLua(sol::state& lua) {
		auto type = lua.new_usertype<CDOTABaseNPC_Hero>(
			"CDOTABaseNPC_Hero",
			sol::base_classes, sol::bases<CDOTABaseNPC, CBaseEntity>()
			);
		type["GetAttributes"] = &CDOTABaseNPC_Hero::GetAttributes;
		type["IsIllusion"] = &CDOTABaseNPC_Hero::IsIllusion;
		lua["CDOTABaseNPC_Hero"] = sol::overload(
			[](CDOTABaseNPC* npc) {return (CDOTABaseNPC_Hero*)npc; },
			[](CBaseEntity* ent) {return (CDOTABaseNPC_Hero*)ent; }
		);
	}
};
