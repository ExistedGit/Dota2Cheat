#pragma once
#include <span>

#include "../Base/NormalClass.h"
#include "../Entities/CHandle.h"
#include "../Base/Definitions.h"
#include "../Netvars.h"
#include "CDOTAItem.h"

class CDOTAUnitInventory : NormalClass {
public:
	// Return an array of 19 entity handles representing slots, if the slot is empty, the handle is invalid(0XFFFFFFFF)
	// Valid handles are ordered by slots, moving an item will change its index inside this array
	std::span<CHandle<CDOTAItem>, 19> GetItems() {
		return std::span<CHandle<CDOTAItem>, 19>{ MemberInline<CHandle<CDOTAItem>>(Netvars::C_DOTA_UnitInventory::m_hItems), 19 };
	}
	bool IsItemInSlot(CHandle<CDOTAItem> item, uint32_t slot);
	int GetItemSlot(CHandle<CDOTAItem> item);
};
