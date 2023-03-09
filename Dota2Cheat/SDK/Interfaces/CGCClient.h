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
	EDOTAGCMsg GetEMsg();
};

class CProtobufSharedObjectBase : public VClass {
public:
	google::protobuf::Message* GetPObject();
};

class CGCClientSharedObjectTypeCache : public VClass {
public:
	CProtobufSharedObjectBase* GetProtobufSO();
};
class CGCClientSharedObjectCache : public VClass {
public:

	CUtlVector<CGCClientSharedObjectTypeCache*> GetTypeCacheList();
};

class CGCClient : public VClass {
public:
	CGCClientSharedObjectCache* GetObjCache();
	VClass* GetGCClientSystem();
};