#include "CDOTABaseNPC.h"

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

