#pragma once
#include "../../Base/VClass.h"
#include "../../Base/CUtlVector.h"
#include "../../Base/Definitions.h"
#include "ItemSchema.h"
#include "../../Protobufs/base_gcmessages.pb.h"

typedef uint8_t	style_index_t;
typedef uint16_t item_definition_index_t;

class CEconItem : public VClass {
	void* vmt2;
public:
	auto GetItemDef() {
		return MemberInline<VClass>(0x8)->CallVFunc<0x58/8, CDOTAItemDefinition*>();
	}

	inline static void (*DeserializeFromProtobufItemFunc)(CEconItem*, CSOEconItem*);
	inline static void (*EnsureCustomDataExistsFunc)(CEconItem*);

	void EnsureCustomDataExists() {
		EnsureCustomDataExistsFunc(this);
	}
	void DeserializeFromProtobufItem(CSOEconItem* proto) {
		DeserializeFromProtobufItemFunc(this, proto);
	}

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

	FIELD(uint8_t, Style, 0x30);
	FIELD(uint8_t, Flag, 0x31);
	FIELD(uint16_t, Class, 0x32);
	FIELD(uint16_t, Slot, 0x34);

	GETTER(uint64_t*, GetCustomData, 0x48);
};
