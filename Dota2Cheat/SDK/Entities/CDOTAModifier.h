#pragma once
#include "../Netvars.h"
#include "../Globals/Interfaces.h"

#include <sol/sol.hpp>

class CDOTABaseNPC;

// server.dll
// CDOTA_Buff
class CDOTAModifier : public VClass {
public:
	GETTER(const char*, GetName, Netvars::CDOTA_Buff::m_name);
	GETTER(float, GetDuration, Netvars::CDOTA_Buff::m_flDuration);
	GETTER(float, GetDieTime, Netvars::CDOTA_Buff::m_flDieTime);
	GETTER(int, GetStackCount, Netvars::CDOTA_Buff::m_iStackCount);
	GETTER(ENT_HANDLE, GetCaster, Netvars::CDOTA_Buff::m_hCaster);
	GETTER(ENT_HANDLE, GetAbility, Netvars::CDOTA_Buff::m_hAbility);

	CDOTABaseNPC* GetOwner();

	static void BindLua(sol::state& lua);
};

