#pragma once
#include "../Base/VClass.h"
#include "../Base/CUtlVector.h"
#include "../Base/Definitions.h"
#include <google/protobuf/message.h>
#include "../Protobufs/dota_gcmessages_msgid.pb.h"
#include "../Enums.h"

typedef uint8_t	style_index_t;
typedef uint16_t item_definition_index_t;

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
		CUtlVector<CEconStyleInfo*>* GetStyles() {
			return Member<CUtlVector<CEconStyleInfo*>*>(0x48);
		}
	};

	GETTER(CEconItemSetDefinition*, GetItemSetDef, 0xA0);
	GETTER(CAssetModifierContainer*, GetAssetModifierContainer, 0xA8);
};

class CEconItem : public VClass { // yes I know it has not just one but two VMTs but it's gotta be like that
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

class CProtobufMsgBase : public VClass {
private:
	void* unk;
public:
	google::protobuf::Message* header;
	EDOTAGCMsg msgID;
	google::protobuf::Message* msg;
};

class IMsgNetPacket : public VClass {
public:
	GETTER(EDOTAGCMsg, GetEMsg, 0x78);

};

struct SOID_t
{
	uint64_t m_unSteamID;
	uint32_t m_iType;
	uint32_t m_iPadding;
};

class CProtobufSharedObjectBase : public VClass {
public:
	google::protobuf::Message* GetPObject() {
		return CallVFunc<9, google::protobuf::Message*>();
	}
};

class CGCClientSharedObjectTypeCache : public VClass {
public:
	CProtobufSharedObjectBase* GetProtobufSO() {
		return *Member< CProtobufSharedObjectBase**>(0x10);
	}
	GETTER(uint32_t, GetEconTypeID, 0x28);
};

class CDOTAGCClientSystem : public VClass {
public:
	void SaveSerializedSOCache() {
		CallVFunc<2>();
	}
};

class CGCClientSharedObjectCache : public VClass {
public:
	GETTER(CUtlVector<CGCClientSharedObjectTypeCache*>, GetTypeCacheList, 0x10);
	GETTER(SOID_t, GetOwner, 0x28);

	bool AddObject(void* sharedObj) {
		return CallVFunc<2, bool>(sharedObj);
	}
};

class ISharedObjectListener : public VClass {
public:
	void SOCreated(SOID_t* soid, void* sharedObj, ESOCacheEvent ev) {
		CallVFunc<0>(soid, sharedObj, ev);
	}
	void SOUpdated(SOID_t* soid, void* sharedObj, ESOCacheEvent ev) {
		CallVFunc<1>(soid, sharedObj, ev);
	}
	GETTER(CGCClientSharedObjectCache*, GetSOCache, 0xA0);
};

class CGCClient : public VClass {
public:
	GETTER(CUtlVector<ISharedObjectListener*>, GetSOListeners, 0x270);

	void DispatchSOUpdated(SOID_t soid, void* sharedObj, ESOCacheEvent ev);

};