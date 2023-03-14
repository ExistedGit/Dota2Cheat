#pragma once
#include <span>

#include "../Base/NormalClass.h"
#include "../Base/Definitions.h"
#include "../Netvars.h"

class CDOTAUnitInventory : NormalClass {
public:
	// Returns an array of 19 entity handles representing slots, if the slot is empty, the handle is invalid(0XFFFFFFFF)
	// Valid handles of items are ordered by slots, i. e. moving an item to backpack will change its index inside this array
	std::span<ENT_HANDLE, 19> GetItems();
	bool IsItemInSlot(ENT_HANDLE item, uint32_t slot);
	int GetItemSlot(ENT_HANDLE item);
};
