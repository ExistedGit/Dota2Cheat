#pragma once
#include "ISharedObjectListener.h"
#include "../../Protobufs/dota_gcmessages_common_lobby.pb.h"

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
	uint64_t GetLobbyId() {
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
	uint64_t unk;
public:
	LobbyData lobbyData;

	// Can't be bothered to check what's at these offsets and rename variables
	// Just made Morphling's IDA paste a bit more appealing
	CDOTALobby* FindLobby() {
		auto v1 = Member<uint32_t>(24);
		if (v1 <= 0)
			return 0;

		int v2 = 0;
		uintptr_t v5 = Member<uintptr_t>(16);
		uint64_t i = 0;
		for (; ; i += 64)
		{
			if (v2 >= 0 && v2 < v1 && v2 <= Member<uint32_t>(44)
				&& *(DWORD*)(i + v5) != v2
				&& *(uintptr_t*)(i + v5 + 24))
				break;

			if (++v2 >= v1)
				return nullptr;
		}
		return *(CDOTALobby**)(i + v5 + 24);
	}
};