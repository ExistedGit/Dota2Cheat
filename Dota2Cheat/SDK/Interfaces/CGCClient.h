#pragma once
#include "../Base/VClass.h"
#include "../Base/CUtlVector.h"
#include "../Base/Definitions.h"
#include <google/protobuf/message.h>
#include "../Protobufs/dota_gcmessages_msgid.pb.h"
#include "../Enums.h"
#include "../Protobufs/custommessages.pb.h"

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

using SOID_t = uint64_t;

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
	GETTER(uint32_t, GetSomeKindOfIndex, 0x28);
};
class CGCClientSharedObjectCache : public VClass {
public:
	GETTER(CUtlVector<CGCClientSharedObjectTypeCache*>, GetTypeCacheList, 0x10);
	SOID_t* GetOwner() {
		return MemberInline<SOID_t>(0x28);
	}
};

class ISharedObjectListener : public VClass {
public:
	void DispatchUpdate(SOID_t* soid, void* sharedObj, ESOCacheEvent ev) {
		CallVFunc<1>(soid, sharedObj, ev);
	}
	GETTER(CGCClientSharedObjectCache*, GetSOCache, 0xA0);
};

class CGCClient : public VClass {
public:
	GETTER(CUtlVector<ISharedObjectListener*>, GetSOListeners, 0x270);

	void DispatchSOUpdated(SOID_t* soid, void* sharedObj, ESOCacheEvent ev);

};