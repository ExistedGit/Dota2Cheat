#include "CDOTABaseNPC.h"

[[nodiscard]]
std::vector<CDOTABaseAbility*> CDOTABaseNPC::GetAbilities() {
	std::vector<CDOTABaseAbility*> result{};
	ENT_HANDLE* hAbilities = MemberInline<ENT_HANDLE>(Netvars::C_DOTA_BaseNPC::m_hAbilities);
	for (int j = 0; j < 35; j++) {
		ENT_HANDLE handle = hAbilities[j];
		if (!HVALID(handle))
			continue;

		if (auto ability = Interfaces::EntitySystem->GetEntity<CDOTABaseAbility>(H2IDX(handle)))
			result.push_back(ability);

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
	auto unitState = Member<int64>(Netvars::C_DOTA_BaseNPC::m_nUnitState64);
	return (unitState & (1Ui64 << (int)state));
}