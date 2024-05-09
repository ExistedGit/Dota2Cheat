#include "CDOTAUnitInventory.h"

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
