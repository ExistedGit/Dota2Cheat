#pragma once
#include "../Base/VClass.h"
#include "../Base/CUtlVector.h"
#include <google/protobuf/message.h>
#include "../Protobufs/dota_gcmessages_msgid.pb.h"

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
	EDOTAGCMsg GetEMsg() {
		return Member<EDOTAGCMsg>(0x78);
	}
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
};
class CGCClientSharedObjectCache : public VClass {
public:

	CUtlVector<CGCClientSharedObjectTypeCache*> GetTypeCacheList() {
		return Member<CUtlVector<CGCClientSharedObjectTypeCache*>>(0x10);
	}
};

class CGCClient : public VClass {
public:
	CGCClientSharedObjectCache* GetObjCache() {
		return Member<CGCClientSharedObjectCache*>(0x450);
	}
	VClass* GetGCClientSystem() {
		return Member<VClass*>(0x768);
	}
};