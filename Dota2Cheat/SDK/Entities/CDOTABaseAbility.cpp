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
	return GetKVValueFor<int>("radius", -1);
}

