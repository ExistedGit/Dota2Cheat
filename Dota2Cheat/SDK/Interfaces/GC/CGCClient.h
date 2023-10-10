#pragma once
#include "../../Base/VClass.h"
#include "../../Base/CUtlVector.h"
#include "../../Base/Definitions.h"
#include <google/protobuf/message.h>
#include "../../Protobufs/dota_gcmessages_msgid.pb.h"
#include "../../Enums.h"
#include "ISharedObjectListener.h"
#include "CDOTAGCLobbyManager.h"

template<typename T = google::protobuf::Message>
class CProtobufMsgBase : public VClass {
private:
	void* unk;
public:
	google::protobuf::Message* header;
	EDOTAGCMsg msgID;
	T* msg;
};

class IMsgNetPacket : public VClass {
public:
	GETTER(EDOTAGCMsg, GetEMsg, 0x78);
};

class CProtobufSharedObjectBase : public VClass {
public:
	VGETTER(google::protobuf::Message*, GetPObject, 9);
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

class CGCClient : public VClass {
public:
	GETTER(CUtlVector<ISharedObjectListener*>, GetSOListeners, 0x270);
	IGETTER(CDOTAGCClientLobbyManager, GetLobbyManager, 0x6c8);

	void DispatchSOUpdated(SOID_t soid, void* sharedObj, ESOCacheEvent ev) {
		auto listeners = GetSOListeners();
		for (auto& listener : listeners)
			listener->SOUpdated(&soid, sharedObj, ev);
	}
	uint64_t GetReadyUpKey() {
		auto lobbyId = ~GetLobbyManager()->FindLobby()->GetLobbyId();
		uint32_t accId = GetSOListeners()[1]->GetSOCache()->GetOwner().m_unSteamID;
		return lobbyId ^ (accId | ((uint64_t)accId << 32));
	}
};