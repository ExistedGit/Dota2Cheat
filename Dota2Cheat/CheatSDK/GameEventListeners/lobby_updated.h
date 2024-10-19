#pragma once
#include "../Data/EntityList.h"

#include "../../Modules/Hacks/AutoAccept.h"

class lobby_updated_l : public IGameEventListener2 {
public:
	void FireGameEvent(CGameEvent* event) override {
		auto gcc = CGCClient::Get();
		auto lobby = gcc->GetLobbyManager()->FindLobby();
		if (lobby && lobby->GetSOLobby()->state() == 4 && gcc->Member<uint32>(0x508) <= 1) {
			Modules::AutoAccept.AcceptMatch();
		};
	}
};