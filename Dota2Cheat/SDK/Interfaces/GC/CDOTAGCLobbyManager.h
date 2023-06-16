#pragma once
#include "ISharedObjectListener.h"
#include "../../Protobufs/dota_gcmessages_common_lobby.pb.h"

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
	uint64_t lobby_id() {
		return Member<VClass*>(0x18)->Member<uint64_t>(0x1D8);
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
public:
	uint64_t unk;
	LobbyData lobbyData;

	CDOTALobby* FindLobby() {
		return Member<LobbyData*>(0x10)[Member<uint32_t>(0x10) - 1].m_dota_lobby;
	}
};