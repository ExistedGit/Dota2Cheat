#pragma once
#include <span>

#include "../Base/NormalClass.h"
#include "../Base/Definitions.h"
#include "../Netvars.h"

class CDOTAUnitInventory : NormalClass {
public:
	// Returns an array of 19 entity handles representing slots, if the slot is empty, the handle is invalid(0XFFFFFFFF)
	// Valid handles of items are ordered by slots, i. e. moving an item to backpack will change its index inside this array
	auto GetItems() {
		return std::span<ENT_HANDLE, 19>{ MemberInline<ENT_HANDLE>(Netvars::C_DOTA_UnitInventory::m_hItems), 19 };
	}
	bool IsItemInSlot(ENT_HANDLE item, uint32_t slot) {
		if (slot > 18)
			return false;

		return GetItems()[slot] == item;
	}
	int GetItemSlot(ENT_HANDLE item) {
		auto items = GetItems();
		auto it = std::find(items.begin(), items.end(), item);
		return it != items.end()
			? it - items.begin() 
			: -1;
	}
};
