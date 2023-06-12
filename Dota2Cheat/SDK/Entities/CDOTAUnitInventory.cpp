#include "CDOTAUnitInventory.h"

// Returns an array of 19 entity handles representing slots, if the slot is empty, the handle is invalid(0XFFFFFFFF)
// Valid handles of items are ordered by slots, i. e. moving an item to backpack will change its index inside this array
std::span<CHandle<CDOTAItem>, 19> CDOTAUnitInventory::GetItems() {
	return std::span<CHandle<CDOTAItem>, 19>{ MemberInline<CHandle<CDOTAItem>>(Netvars::C_DOTA_UnitInventory::m_hItems), 19 };
}

bool CDOTAUnitInventory::IsItemInSlot(CHandle<CDOTAItem> item, uint32_t slot) {
	if (slot > 18)
		return false;

	return GetItems()[slot] == item;
}

int CDOTAUnitInventory::GetItemSlot(CHandle<CDOTAItem> item) {
	if (!item.IsValid())
		return -1;

	auto items = GetItems();
	auto it = std::find(items.begin(), items.end(), item);
	return it != items.end()
		? it - items.begin()
		: -1;
}
