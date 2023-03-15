#pragma once
#include "CDOTABaseAbility.h"

// Current stat of Power Treads/Vambrace
enum class ItemStat_t {
	STRENGTH = 0,
	INTELLIGENCE,
	AGILITY
};

class CDOTAItem : public CDOTABaseAbility {
public:
	GETTER(int, GetCurrentCharges, Netvars::C_DOTA_Item::m_iCurrentCharges);
	// For PT and Vambrace
	GETTER(ItemStat_t, GetItemStat, 0x670);
};