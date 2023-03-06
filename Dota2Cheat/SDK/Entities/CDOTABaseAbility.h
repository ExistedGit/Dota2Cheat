#pragma once
#include "CBaseEntity.h"



class CDOTABaseAbility : public CBaseEntity {
public:
	typedef double(__fastcall* GetLevelSpecialValueForFn)(void* thisptr, int abilityIndex, const char* value, int level);
	static inline GetLevelSpecialValueForFn GetLevelSpecialValueForFunc{};

	float GetCooldown() {
		return Member<float>(Netvars::C_DOTABaseAbility::m_fCooldown);
	}
	int GetCharges() {
		return Member<int>(Netvars::C_DOTABaseAbility::m_nAbilityCurrentCharges);
	}
	int GetManaCost() {
		return Member<int>(Netvars::C_DOTABaseAbility::m_iManaCost);
	}

	// Xref "Script_GetCastRangeBonus" in x64dbg to a lea rax
	// Below there will be "hTarget" or something, above that will be a lea rcx, [XXX] with a function
	// At the end there are two calls to [rdi + 0x798] and [rdi + 0x7a0], first it gets the range, then the bonus
	// I currently could not get GetCastRange to work as a standalone vfunc
	// found via dynamical analysis
	int GetCastRangeBonus() {
		return CallVFunc<244, int>(nullptr, nullptr, nullptr);
	}

	int GetEffectiveCastRange() {
		return GetCastRange() + GetCastRangeBonus();
	}
	int GetCastRange() {
		return GetLevelSpecialValueFor<int>("AbilityCastRange");
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
