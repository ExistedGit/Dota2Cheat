#pragma once
#include "ISharedObjectListener.h"
#include "Common.h"
#include <dota_gcmessages_common_lobby.pb.h>

// Reversed in cooperation with Morphling

struct dota_dynamic_lobby_t {
	void* pad;
	CSODOTALobby* dynamic_lobby;
};
struct dota_static_lobby_t {
	void* pad;
	CSODOTALobby* static_lobby;
};

class CDOTALobby : public VClass {
public:
	CSODOTALobby* GetSOLobby() const {
		return Member<CProtobufSharedObjectBase*>(0x18)->GetPObj<CSODOTALobby>();
	}
};

struct LobbyData {
	uint64_t unk; // 0x0
	uint64_t unk2; // 0x8
	uint64_t unk3; // 0x10
	CDOTALobby* m_dota_lobby; // 0x18
	dota_dynamic_lobby_t* m_dynamic; //0x20
	dota_static_lobby_t* m_static; // 0x28
};


class CDOTAGCClientLobbyManager : public ISharedObjectListener {
	uint64_t unk;
public:
	LobbyData lobbyData;

	// Can't be bothered to check what's at these offsets and rename variables
	// Just made Morphling's IDA paste a bit more appealing
	CDOTALobby* FindLobby() const;
};