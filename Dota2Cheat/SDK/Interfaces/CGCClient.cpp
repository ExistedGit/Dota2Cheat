#include "CGCClient.h"

CGCClientSharedObjectCache* CGCClient::GetObjCache() {
	return Member<CGCClientSharedObjectCache*>(0x450);
}

VClass* CGCClient::GetGCClientSystem() {
	return Member<VClass*>(0x768);
}

CUtlVector<CGCClientSharedObjectTypeCache*> CGCClientSharedObjectCache::GetTypeCacheList() {
	return Member<CUtlVector<CGCClientSharedObjectTypeCache*>>(0x10);
}

CProtobufSharedObjectBase* CGCClientSharedObjectTypeCache::GetProtobufSO() {
	return *Member< CProtobufSharedObjectBase**>(0x10);
}

google::protobuf::Message* CProtobufSharedObjectBase::GetPObject() {
	return CallVFunc<9, google::protobuf::Message*>();
}

EDOTAGCMsg IMsgNetPacket::GetEMsg() {
	return Member<EDOTAGCMsg>(0x78);
}
