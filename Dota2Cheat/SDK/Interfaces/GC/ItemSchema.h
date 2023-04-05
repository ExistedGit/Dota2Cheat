#pragma once
#include "../../Base/VClass.h"
#include "../../Base/CUtlVector.h"
#include "../../Base/Definitions.h"

class CDOTAItemDefinition : public VClass {
public:
	// Localization strings
	GETTER(const char*, GetName, 0x40);
	GETTER(const char*, GetWearableType, 0x48);
	GETTER(const char*, GetDesc, 0x50);
	GETTER(const char*, GetEnglishName, 0xf0);

	struct CEconItemSetDefinition : public VClass {
		const char* setName, * setLocalizedName;
	};
	struct CEconStyleInfo : public VClass {
		GETTER(const char*, GetName, 0x18);
		GETTER(const char*, GetModelPath, 0x20);
	};
	struct CAssetModifierContainer : public VClass {
		GETTER(CUtlVector<CEconStyleInfo*>*, GetStyles, 0x48);
	};

	GETTER(CEconItemSetDefinition*, GetItemSetDef, 0xA0);
	GETTER(CAssetModifierContainer*, GetAssetModifierContainer, 0xA8);
	// Returns the slot's index + 1
	GETTER(uint32_t, GetSlot, 0x188);
	GETTER(uint32_t, GetClass, 0x18C);
};

struct ItemDefNode {
	uint32_t unDefIndex, unk0;
	CDOTAItemDefinition* itemDef;
	uintptr_t unk1;
};

class CDOTAItemSchema : public VClass {
public:
	inline static CDOTAItemDefinition* (*GetItemDefByIndex)(VClass* itemSchema, uint32_t index) = nullptr;

	GETTER(CUtlVector<ItemDefNode>, GetItemDefinitions, 0x48);
};
