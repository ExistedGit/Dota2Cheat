#pragma once
#include "CBaseEntity.h"

class CDOTABaseAbility : public CBaseEntity {
public:
	typedef double(__fastcall* GetLevelSpecialValueForFn)(void* thisptr, int abilityIndex, const char* value, int level);
	static inline GetLevelSpecialValueForFn GetLevelSpecialValueForFunc{};

	GETTER(bool, IsHidden, Netvars::C_DOTABaseAbility::m_bHidden);
	GETTER(float, GetCooldown, Netvars::C_DOTABaseAbility::m_fCooldown);
	GETTER(int, GetLevel, Netvars::C_DOTABaseAbility::m_iLevel);
	GETTER(int, GetMaxLevel, Netvars::C_DOTABaseAbility::m_iMaxLevel);
	GETTER(int, GetCharges, Netvars::C_DOTABaseAbility::m_nAbilityCurrentCharges);
	GETTER(int, GetManaCost, Netvars::C_DOTABaseAbility::m_iManaCost);

	// xref: "GetCastRange" to lea rcx, above that is lea rax, [XXXXXXXXX]
	// In the end of the func is a call to [rcx + 0x???] <--- that divided by 8 gives you the index
	int GetCastRange() {
		// Still using GetLevelSpecialValueFor because that has clearer usage
		return GetLevelSpecialValueFor<int>("AbilityCastRange");
	}
	// Goes right after GetCastRange ^
	int GetCastRangeBonus() {
		return CallVFunc<VTableIndexes::CDOTABaseAbility::GetCastRangeBonus, int>(nullptr, nullptr, nullptr);
	}

	int GetEffectiveCastRange() {
		return GetCastRange() + GetCastRangeBonus();
	}

	template<typename T = double>
	T GetLevelSpecialValueFor(const char* valName, int level = -1) {
		return (T)GetLevelSpecialValueForFunc(nullptr, GetIndex(), valName, level);
	}

	int GetAOERadius() {
		return GetLevelSpecialValueFor<int>("radius", -1);
	}

};
