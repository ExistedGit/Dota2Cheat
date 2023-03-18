#include "CDOTABaseAbility.h"

// xref: "GetCastRange" to lea rcx, above that is lea rax, [XXXXXXXXX]
// In the end of the func is a call to [rcx + 0x???] <--- that divided by 8 gives you the index

int CDOTABaseAbility::GetCastRange() {
	// Still using GetLevelSpecialValueFor because that has clearer usage
	return GetKVValueFor<int>("AbilityCastRange");
}

// Goes right after GetCastRange ^

int CDOTABaseAbility::GetCastRangeBonus() {
	return CallVFunc<VTableIndexes::CDOTABaseAbility::GetCastRangeBonus, int>(nullptr, nullptr, nullptr);
}

int CDOTABaseAbility::GetEffectiveCastRange() {
	return GetCastRange() + GetCastRangeBonus();
}

int CDOTABaseAbility::GetAOERadius() {
	return GetLevelSpecialValueFor<int>("radius", -1);
}

void CDOTABaseAbility::BindLua(sol::state& lua) {
	auto type = lua.new_usertype< CDOTABaseAbility>("CDOTABaseAbility", sol::base_classes, sol::bases<CBaseEntity>());
	type["IsToggled"] = &CDOTABaseAbility::IsToggled;
	type["IsHidden"] = &CDOTABaseAbility::IsHidden;
	type["GetCooldown"] = &CDOTABaseAbility::GetCooldown;
	type["GetCooldownLength"] = &CDOTABaseAbility::GetCooldownLength;
	type["GetLevel"] = &CDOTABaseAbility::GetLevel;
	type["GetMaxLevel"] = &CDOTABaseAbility::GetMaxLevel;
	type["GetCharges"] = &CDOTABaseAbility::GetCharges;
	type["GetChargeRestoreCooldown"] = &CDOTABaseAbility::GetChargeRestoreCooldown;
	type["GetManaCost"] = &CDOTABaseAbility::GetManaCost;
	type["IsInAbilityPhase"] = &CDOTABaseAbility::IsInAbilityPhase;
	type["GetCastRange"] = &CDOTABaseAbility::GetCastRange;
	type["GetCastRangeBonus"] = &CDOTABaseAbility::GetCastRangeBonus;
	type["GetEffectiveCastRange"] = &CDOTABaseAbility::GetEffectiveCastRange;
	type["GetKVValueFor"] = &CDOTABaseAbility::GetKVValueFor<float>;
	type["GetLevelSpecialValueFor"] = &CDOTABaseAbility::GetLevelSpecialValueFor<float>;
	type["GetAOERadius"] = &CDOTABaseAbility::GetAOERadius;
}

