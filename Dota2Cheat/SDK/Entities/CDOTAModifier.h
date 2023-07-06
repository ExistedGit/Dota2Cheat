#pragma once
#include "../Netvars.h"
#include "../Globals/Interfaces.h"
#include "CHandle.h"

#include "CBaseEntity.h"

class CDOTABaseNPC;

// server.dll
// CDOTA_Buff
class CDOTAModifier : public VClass {
public:
	GETTER(const char*, GetName, Netvars::CDOTA_Buff::m_name);
	GETTER(float, GetDuration, Netvars::CDOTA_Buff::m_flDuration);
	GETTER(float, GetDieTime, Netvars::CDOTA_Buff::m_flDieTime);
	GETTER(int, GetStackCount, Netvars::CDOTA_Buff::m_iStackCount);
	GETTER(bool, IsAura, Netvars::CDOTA_Buff::m_bIsAura);
	GETTER(DOTA_GC_TEAM, GetTeam, Netvars::CDOTA_Buff::m_iTeam);
	bool IsSameTeam(CBaseEntity* ent) {
		return ent->GetTeam() == GetTeam();
	}
	GETTER(CHandle<CDOTABaseNPC>, GetCaster, Netvars::CDOTA_Buff::m_hCaster);
	GETTER(CHandle<CDOTABaseNPC>, GetAbility, Netvars::CDOTA_Buff::m_hAbility);

	CDOTABaseNPC* GetOwner();
};

