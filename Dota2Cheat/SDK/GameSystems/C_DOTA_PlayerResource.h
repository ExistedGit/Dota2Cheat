#pragma once
#include "../Base/VClass.h"
#include "../Netvars.h"
#include "../Base/CUtlVector.h"

class PlayerResourcePlayerData_t :public VClass {
	char pad[0xc8];
public:
	uint32_t GetPlayerSlot() {
		return Member<uint32_t>(Netvars::PlayerResourcePlayerData_t::m_nPlayerSlot);
	}
};
class C_DOTA_PlayerResource : public VClass {
public:
	CUtlVector<PlayerResourcePlayerData_t> GetVecPlayerTeamData() {
		return Member<CUtlVector<PlayerResourcePlayerData_t>>(Netvars::C_DOTA_PlayerResource::m_vecPlayerTeamData);
	}
	ENT_HANDLE PlayerSlotToHandle(int slot) {
		return MemberInline<int>(Netvars::C_DOTA_PlayerResource::m_playerIDToPlayer)[slot];
	}
};