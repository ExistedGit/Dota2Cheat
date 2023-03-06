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
	// For PT and Vambrace
	ItemStat_t GetItemStat() {
		return Member<ItemStat_t>(0x670);
	}

	// For items like Armlet
	bool IsToggled() {
		return Member<bool>(Netvars::C_DOTABaseAbility::m_bToggleState);
	}
};