#pragma once
#include "sdk.h"
#include "CUtlVector.h"
#include "SDK/CNetworkMessages.h"
#include <google/protobuf/message.h>

class CProtobufMsgBase : public VClass {
private:
	void* unk;
public:
	google::protobuf::Message* header;
	EDOTAGCMsg msgID;
private:
	char pad[4];
public:
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
};