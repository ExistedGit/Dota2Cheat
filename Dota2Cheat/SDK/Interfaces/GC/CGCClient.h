#pragma once
#include "../../Base/VClass.h"
#include "../../Base/CUtlVector.h"
#include "../../Base/Definitions.h"
#include "../../Enums.h"
#include "ISharedObjectListener.h"
#include "CDOTAGCLobbyManager.h"
#include <google/protobuf/message.h>
#include <dota_gcmessages_msgid.pb.h>

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
	CProtobufSharedObjectBase* GetProtobufSO() const {
		return *Member<CProtobufSharedObjectBase**>(0x10);
	}
	GETTER(uint32_t, GetEconTypeID, 0x28);
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
	IGETTER(CDOTAGCClientLobbyManager, GetLobbyManager, 0x700);

	void DispatchSOUpdated(SOID_t soid, void* sharedObj, ESOCacheEvent ev) {
		auto listeners = GetSOListeners();
		for (auto& listener : listeners)
			listener->SOUpdated(&soid, sharedObj, ev);
	}

	// via xrefs "ui.matchmaking_accept" or "ui.matchmaking_cancel" find CDOTAGCClientSystem::SendReadyUpMessageForCurrentLobby
	// in there you will see all of the required components in this order:
	// finds CDOTALobby* in lobby manager
	// gets 32bit SteamID
	// gets lobby id from lobby
	uint64_t GetReadyUpKey() const {
		auto lobbyId = ~GetLobbyManager()->FindLobby()->GetLobbyId();
		uint32_t accId = GetSOListeners()[1]->GetSOCache()->GetOwner().m_unSteamID;
		return lobbyId ^ (accId | ((uint64_t)accId << 32));
	}

	static CGCClient* Get();
};