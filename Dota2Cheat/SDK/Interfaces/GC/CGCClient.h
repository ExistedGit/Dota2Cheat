#pragma once
#include "../../Base/VClass.h"
#include "../../Base/CUtlVector.h"
#include "../../Base/Definitions.h"
#include <google/protobuf/message.h>
#include "../../Protobufs/dota_gcmessages_msgid.pb.h"
#include "../../Enums.h"


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