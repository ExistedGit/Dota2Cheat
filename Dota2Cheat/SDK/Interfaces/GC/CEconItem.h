#pragma once
#include "../../Base/VClass.h"
#include "../../Base/CUtlVector.h"
#include "../../Base/Definitions.h"

typedef uint8_t	style_index_t;
typedef uint16_t item_definition_index_t;

class CEconItem : public VClass {
	void* vmt2;
public:
	uint64_t m_ulID; // 0x10 (Item ID)
	uint64_t unknown; // 0x18
	uint32_t m_unAccountID; // 0x20 (Account owner ID)
	uint32_t m_unInventory; // 0x24
	item_definition_index_t m_unDefIndex; // 0x28
	uint8_t m_unLevel; // not sure
	uint8_t m_nQuality;
	uint8_t m_unFlags; // not sure
	uint8_t m_unOrigin;
	style_index_t m_unStyle;

	uint8_t& Style() {
		return Field<uint8_t>(0x30);
	}
	uint8_t& Flag() {
		return Field<uint8_t>(0x31);
	}
	uint16_t& Class() {
		return Field<uint16_t>(0x32);
	}
	uint16_t& Slot() {
		return Field<uint16_t>(0x34);
	}
};
