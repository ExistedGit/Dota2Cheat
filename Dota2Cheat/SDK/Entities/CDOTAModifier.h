#pragma once
#include "../Base/CUtlVector.h"
#include "../Netvars.h"
#include "CHandle.h"
#include "CBaseEntity.h"

class CDOTABaseNPC;

struct CDOTA_BuffParticle {
	uint32_t particleHandle, unk = 10000, entIdx;
};

// server.dll
// CDOTA_Buff
class CDOTAModifier : public VClass {
public:
	GETTER(const char*, GetName, Netvars::CDOTA_Buff::m_name);
	GETTER(float, GetDuration, Netvars::CDOTA_Buff::m_flDuration);
	GETTER(float, GetDieTime, Netvars::CDOTA_Buff::m_flDieTime);
	GETTER(int, GetStackCount, Netvars::CDOTA_Buff::m_iStackCount);
	GETTER(bool, IsAura, Netvars::CDOTA_Buff::m_bIsAura);
	FIELD(CUtlVector<CDOTA_BuffParticle>, Particles, Netvars::CDOTA_Buff::m_iParticles);
	GETTER(DOTA_GC_TEAM, GetTeam, Netvars::CDOTA_Buff::m_iTeam);
	GETTER(CHandle<CDOTABaseNPC>, GetCasterHandle, Netvars::CDOTA_Buff::m_hCaster);
	GETTER(CHandle<CDOTABaseAbility>, GetAbilityHandle, Netvars::CDOTA_Buff::m_hAbility);
	GETTER(CHandle<CDOTABaseNPC>, GetOwnerHandle, Netvars::CDOTA_Buff::m_hParent);

	bool IsSameTeam(CBaseEntity* ent) {
		return ent->GetTeam() == GetTeam();
	}

	CDOTABaseNPC* GetOwner() {
		return GetOwnerHandle();
	}
	CDOTABaseNPC* GetCaster() {
		return GetCasterHandle();
	}
	CDOTABaseAbility* GetAbility() {
		return GetAbilityHandle();
	}
	VGETTER(const char*, GetTexture, 3);
};

