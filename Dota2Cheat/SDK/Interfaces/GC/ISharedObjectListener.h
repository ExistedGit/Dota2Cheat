#pragma once
#include "../../Base/VClass.h"
#include "../../Base/Definitions.h"
#include "../../Enums.h"

class CGCClientSharedObjectCache;

struct SOID_t
{
	uint64_t m_unSteamID;
	uint32_t m_iType;
	uint32_t m_iPadding;
};

class ISharedObjectListener : public VClass {
public:
	void SOCreated(SOID_t* soid, void* sharedObj, ESOCacheEvent ev) {
		CallVFunc<0>(soid, sharedObj, ev);
	}
	void SOUpdated(SOID_t* soid, void* sharedObj, ESOCacheEvent ev) {
		CallVFunc<1>(soid, sharedObj, ev);
	}

	// Only for PlayerInventory
	GETTER(CGCClientSharedObjectCache*, GetSOCache, 0xA0);
};