#pragma once
#include "CDOTABaseNPC.h"

class CDOTABaseNPC_Hero : public CDOTABaseNPC {
public:
	struct HeroAttributes {
		float strength, agility, intellect;
	};

	GETTER(HeroAttributes, GetAttributes, Netvars::C_DOTA_BaseNPC_Hero::m_flStrengthTotal);

	bool IsIllusion() {
		return HVALID(Member<ENT_HANDLE>(Netvars::C_DOTA_BaseNPC_Hero::m_hReplicatingOtherHeroModel));
	}
};
