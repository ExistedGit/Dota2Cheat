#pragma once
#include "CBaseEntity.h"

class CDOTABaseAbility : public CBaseEntity {
public:
	typedef double(__fastcall* GetLevelSpecialValueForFn)(void* thisptr, int abilityIndex, const char* value, int level);
	static inline GetLevelSpecialValueForFn GetLevelSpecialValueForFunc{};

	float GetCooldown() {
		return Member<float>(Netvars::C_DOTABaseAbility::m_fCooldown);
	}
	GETTER(int, GetLevel, Netvars::C_DOTABaseAbility::m_iLevel);

	int GetCharges() {
		return Member<int>(Netvars::C_DOTABaseAbility::m_nAbilityCurrentCharges);
	}
	int GetManaCost() {
		return Member<int>(Netvars::C_DOTABaseAbility::m_iManaCost);
	}

	
	// xref: "GetCastRange" to lea rcx, above that is lea rax, [XXXXXXXXX]
	// In the end of the func is a call to [rcx + 0x???] <--- that divided by 8 gives you the index
	int GetCastRange() {
		return GetLevelSpecialValueFor<int>("AbilityCastRange");
	}
	// Goes right after GetCastRange
	int GetCastRangeBonus() {
		
		//return Function(0x00007FFE1495DEA0).Execute<int>(nullptr, GetHandle());
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

	bool IsHidden() {
		return Member<bool>(Netvars::C_DOTABaseAbility::m_bHidden);
	}
};
