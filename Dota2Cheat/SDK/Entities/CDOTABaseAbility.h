#pragma once
#include "CBaseEntity.h"

// Current stat of Power Treads/Vambrace
enum class ItemStat_t {
	STRENGTH = 0,
	INTELLIGENCE,
	AGILITY
};

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

	// For PT and Vambrace
	ItemStat_t GetItemStat() {
		return Member<ItemStat_t>(0x670);
	}

	// For items like Armlet
	bool IsToggled() {
		return Member<bool>(Netvars::C_DOTABaseAbility::m_bToggleState);
	}

	//int GetCastRange() {
	//	auto pos = GetPos();
	//	return CallVFunc<0x798/8, int>(&pos, nullptr, *(uintptr_t*)this);
	//}

	// Xref "Script_GetCastRangeBonus" in x64dbg to a lea rax
	// Below there will be "hTarget" or something, above that will be a lea rcx, [XXX] with a function
	// At the end there are two calls to [rdi + 0x798] and [rdi + 0x7a0], first it gets the range, then the bonus
	// I currently could not get GetCastRange to work as a standalone vfunc
	// found via dynamical analysis
	int GetCastRangeBonus() {
		return CallVFunc<244, int>(nullptr, nullptr, nullptr);
	}

	int GetEffectiveCastRange() {
		return GetLevelSpecialValueFor("AbilityCastRange") + GetCastRangeBonus();
	}

	// A bit tricky to reverse, done via x64dbg
	// Xref GetCastRange to a lea, rcx instruction, before it is a lea r9, [XXX] <- Follow in Disassembler > Constant: XXX
	// The return register for x64 fastcall is RAX, so breakpoint it and call with nullptr and entity index of your item
	// Step over until you see RAX change to the hex representation of the item's range(in my case Hand of Midas has 600 dec 0x258 hex)
	// There will be a call instruction that will change RAX to the radius, double-click it
	// At the end of the call are:
	// mov rbx, [rcx + 0x568]     <- I forgot the actual registers used here
	// mov rax, [rbx + 0x100]
	// Which means "dereference a pointer to an object on offset 0x568, then dereference a pointer to an int on 0x100 offset of that object"
	int GetCastRange() {
		auto infoObj = Member<VClass*>(0x568);
		return *infoObj->Member<int*>(0x100); // Weird key-value structure
	}
	template<typename T = double>
	T GetLevelSpecialValueFor(const char* valName, int level = -1) {
		return (T)GetLevelSpecialValueForFunc(nullptr, H2IDX(GetIdentity()->entHandle), valName, level);
	}

	int GetAOERadius() {
		return static_cast<int>(GetLevelSpecialValueForFunc(nullptr, H2IDX(GetIdentity()->entHandle), "radius", -1));
	}

	bool IsHidden() {
		return Member<bool>(Netvars::C_DOTABaseAbility::m_bHidden);
	}
};
