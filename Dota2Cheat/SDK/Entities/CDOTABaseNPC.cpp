#include "CDOTABaseNPC.h"

[[nodiscard]]
std::vector<CDOTABaseAbility*> CDOTABaseNPC::GetAbilities() {
	std::vector<CDOTABaseAbility*> result{};
	ENT_HANDLE* hAbilities = MemberInline<ENT_HANDLE>(Netvars::C_DOTA_BaseNPC::m_hAbilities);
	for (int j = 0; j < 35; j++) {
		ENT_HANDLE handle = hAbilities[j];
		if (!HVALID(handle))
			continue;

		result.push_back(Interfaces::EntitySystem->GetEntity<CDOTABaseAbility>(
			H2IDX(handle)
			));
	}
	return result;
}

CDOTAItem* CDOTABaseNPC::FindItemBySubstring(const char* str) {
	if (!str)
		return nullptr;

	for (const auto& item : GetItems())
		if (
			item
			&& item->GetIdentity()->GetName()
			&& strstr(item->GetIdentity()->GetName(), str)
			)
			return item;

	return nullptr;
}

CDOTAUnitInventory* CDOTABaseNPC::GetInventory() {
	return MemberInline<CDOTAUnitInventory>(Netvars::C_DOTA_BaseNPC::m_Inventory);
}

[[nodiscard]]
std::vector<CDOTAItem*> CDOTABaseNPC::GetItems() {
	std::vector<CDOTAItem*> result{};
	CDOTAUnitInventory* inv = GetInventory();
	if (!inv)
		return result;

	auto itemsHandle = inv->GetItems();
	for (auto& handle : itemsHandle) {
		if (!HVALID(handle))
			continue;

		result.push_back(Interfaces::EntitySystem->GetEntity<CDOTAItem>(H2IDX(handle)));
	}

	return result;
}

bool CDOTABaseNPC::HasState(ModifierState state) {
	auto unitState = Member<int64>(Netvars::C_DOTA_BaseNPC::m_nUnitDebuffState);
	return (unitState & ((int)pow(2, (int)state)));
}

float CDOTABaseNPC::GetAttackSpeed() {
	return 1 + CallVFunc<295, float>();
}

void CDOTABaseNPC::BindLua(sol::state& lua) {
	auto type = lua.new_usertype<CDOTABaseNPC>("CDOTABaseNPC", sol::base_classes, sol::bases<CBaseEntity>());

	type["GetModifierManager"] = &CDOTABaseNPC::GetModifierManager;
	type["IsTargetable"] = &CDOTABaseNPC::IsTargetable;
	type["IsWaitingToSpawn"] = &CDOTABaseNPC::IsWaitingToSpawn;
	type["IsAncient"] = &CDOTABaseNPC::IsAncient;
	type["IsRoshan"] = &CDOTABaseNPC::IsRoshan;
	type["GetAttackRange"] = &CDOTABaseNPC::GetAttackRange;
	type["GoalEntity"] = &CDOTABaseNPC::GoalEntity;
	type["GetUnitName"] = &CDOTABaseNPC::GetUnitName;
	type["GetAbilities"] = &CDOTABaseNPC::GetAbilities;
	type["FindItemBySubstring"] = &CDOTABaseNPC::FindItemBySubstring;
	type["GetInventory"] = &CDOTABaseNPC::GetInventory;
	type["GetItems"] = &CDOTABaseNPC::GetItems;;
	type["GetMana"] = &CDOTABaseNPC::GetMana;
	type["GetMaxMana"] = &CDOTABaseNPC::GetMaxMana;
	type["GetBaseAttackTime"] = &CDOTABaseNPC::GetBaseAttackTime;
	type["GetHullRadius"] = &CDOTABaseNPC::GetHullRadius;
	type["GetAttackSpeed"] = &CDOTABaseNPC::GetAttackSpeed;
	lua["CDOTABaseNPC"] = [](CBaseEntity* ent) { return (CDOTABaseNPC*)ent; };
}
