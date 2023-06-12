#pragma once
#include <span>

#include "../Base/NormalClass.h"
#include "../Entities/CHandle.h"
#include "../Base/Definitions.h"
#include "../Netvars.h"
#include "CDOTAItem.h"

class CDOTAUnitInventory : NormalClass {
public:
	// Returns an array of 19 entity handles representing slots, if the slot is empty, the handle is invalid(0XFFFFFFFF)
	// Valid handles of items are ordered by slots, i. e. moving an item to backpack will change its index inside this array
	std::span<CHandle<CDOTAItem>, 19> GetItems();
	bool IsItemInSlot(CHandle<CDOTAItem> item, uint32_t slot);
	int GetItemSlot(CHandle<CDOTAItem> item);
};
