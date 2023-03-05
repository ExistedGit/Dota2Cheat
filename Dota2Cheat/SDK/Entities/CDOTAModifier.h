#pragma once
#include "../Netvars.h"
#include "../Globals/Interfaces.h"

#include <sol/sol.hpp>

class CDOTABaseNPC;

// server.dll
// CDOTA_Buff
class CDOTAModifier : public VClass {
public:
	const char* GetName() {
		return Member<const char*>(Netvars::CDOTA_Buff::m_name);
	}

	float GetDuration() {
		return Member<float>(Netvars::CDOTA_Buff::m_flDuration);
	}

	float GetDieTime() {
		return Member<float>(Netvars::CDOTA_Buff::m_flDieTime);
	}

	ENT_HANDLE GetCaster() {
		return Member<ENT_HANDLE>(Netvars::CDOTA_Buff::m_hCaster);
	}

	ENT_HANDLE GetAbility() {
		return Member<ENT_HANDLE>(Netvars::CDOTA_Buff::m_hAbility);
	}

	CDOTABaseNPC* GetOwner() {
		return Interfaces::EntitySystem->GetEntity<CDOTABaseNPC>(
			H2IDX(
				Member<ENT_HANDLE>(Netvars::CDOTA_Buff::m_hParent)
			)
			);
	}

	static void BindLua(sol::state& lua) {
		auto type = lua.new_usertype<CDOTAModifier>("CDOTAModifier");

		type["GetName"] = &CDOTAModifier::GetName;
		type["GetDuration"] = &CDOTAModifier::GetDuration;
		type["GetDieTime"] = &CDOTAModifier::GetDieTime;
		type["GetCaster"] = &CDOTAModifier::GetCaster;
		type["GetAbility"] = &CDOTAModifier::GetAbility;
		type["GetOwner"] = &CDOTAModifier::GetOwner;
	}
};

