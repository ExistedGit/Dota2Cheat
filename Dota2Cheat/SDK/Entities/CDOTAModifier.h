#pragma once
#include "../Base/CUtlVector.h"
#include "../Netvars.h"
#include "CHandle.h"
#include "CBaseEntity.h"

class CDOTABaseNPC;

struct CDOTA_BuffParticle {
	uint32_t particleHandle, unk = 10000, entIdx;
};

// schema class server.dll/CDOTA_Buff
class CDOTAModifier : public VClass {
public:
	GETTER(const char*, GetName, Netvars::CDOTA_Buff::m_name);
	GETTER(float, GetDuration, Netvars::CDOTA_Buff::m_flDuration);
	GETTER(float, GetDieTime, Netvars::CDOTA_Buff::m_flDieTime);
	GETTER(int, GetStackCount, Netvars::CDOTA_Buff::m_iStackCount);
	GETTER(bool, IsAura, Netvars::CDOTA_Buff::m_bIsAura);
	GETTER(DOTA_GC_TEAM, GetTeam, Netvars::CDOTA_Buff::m_iTeam);
	GETTER(CHandle<CDOTABaseNPC>, GetCaster, Netvars::CDOTA_Buff::m_hCaster);
	GETTER(CHandle<CDOTABaseAbility>, GetAbility, Netvars::CDOTA_Buff::m_hAbility);
	GETTER(CHandle<CDOTABaseNPC>, GetOwner, Netvars::CDOTA_Buff::m_hParent);
	FIELD(CUtlVector<CDOTA_BuffParticle>, Particles, Netvars::CDOTA_Buff::m_iParticles);

	bool IsSameTeam(CBaseEntity* ent) const {
		return ent->GetTeam() == GetTeam();
	}

	VGETTER(const char*, GetTexture, 3);
};

