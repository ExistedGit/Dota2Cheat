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
		int v1; // r9d
		int v2; // eax
		__int64 i; // rdx
		__int64 v5; // rcx
		__int64 a1 = (uintptr_t)this;

		v1 = Member<uint32_t>(24);
		v2 = 0;
		if (v1 <= 0)
			return 0;
		for (i = 0; ; i += 64)
		{
			if (v2 >= 0 && v2 < v1 && v2 <= *(DWORD*)(a1 + 44))
			{
				v5 = *(uintptr_t*)(a1 + 16);
				if (*(DWORD*)(i + v5) != v2)
				{
					if (*(uintptr_t*)(i + v5 + 24))
						break;
				}
			}
			if (++v2 >= v1)
				return 0;
		}
		return *(CDOTALobby**)(((__int64)v2 << 6) + v5 + 24);
	}
};