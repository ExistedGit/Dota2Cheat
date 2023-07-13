#include "CDOTABaseNPC.h"

[[nodiscard]]
std::vector<CDOTABaseAbility*> CDOTABaseNPC::GetAbilities() {
	std::vector<CDOTABaseAbility*> result{};
	auto hAbilities = MemberInline<CHandle<CDOTABaseAbility>>(Netvars::C_DOTA_BaseNPC::m_hAbilities);
	for (int j = 0; j < 35; j++) {
		auto handle = hAbilities[j];
		if (!handle.IsValid())
			continue;

		if (auto ability = handle)
			result.push_back(ability);
	}
	return result;
}

[[nodiscard]]
std::vector<CDOTAItem*> CDOTABaseNPC::GetItems() {
	std::vector<CDOTAItem*> result{};
	CDOTAUnitInventory* inv = GetInventory();
	if (!inv)
		return result;

	auto itemsHandle = inv->GetItems();
	for (auto& handle : itemsHandle) {
		if (!handle.IsValid())
			continue;

		result.push_back(handle);
	}

	return result;
}

