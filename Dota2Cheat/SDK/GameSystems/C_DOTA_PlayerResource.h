#pragma once
#include "../Entities/CDOTAPlayerController.h"
#include "../Base/Definitions.h"

class PlayerResourcePlayerData_t : public VClass {
	char pad[0xc8];
public:
	GETTER(uint32_t, GetPlayerSlot, Netvars::PlayerResourcePlayerData_t::m_nPlayerSlot);
};

struct PlayerResourceTeamData_t : public VClass {
	PAD(0x1d8);
public:
	FIELD(uint32_t, SelectedHeroBadgeXP, Netvars::PlayerResourcePlayerTeamData_t::m_unSelectedHeroBadgeXP);
};

class C_DOTA_PlayerResource : public VClass {
public:
	GETTER(CUtlVector<PlayerResourcePlayerData_t>, GetVecPlayerData, Netvars::C_DOTA_PlayerResource::m_vecPlayerData);
	GETTER(CUtlVector<PlayerResourceTeamData_t>, GetVecPlayerTeamData, Netvars::C_DOTA_PlayerResource::m_vecPlayerTeamData);
	IGETTER(CHandle<CDOTAPlayerController>, GetPlayerIDToPlayerTable, Netvars::C_DOTA_PlayerResource::m_playerIDToPlayer);

	auto PlayerIDToHandle(int id) {
		return GetPlayerIDToPlayerTable()[id];
	}
};