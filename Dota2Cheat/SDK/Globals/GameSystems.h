#pragma once
#include "../../include.h"
#include "Interfaces.h"
#include "Context.h"

#include "../GameSystems/C_DOTAGameRules.h"
#include "../GameSystems/C_DOTA_PlayerResource.h"
#include "../GameSystems/CDOTAParticleManager.h"
#include "../GameSystems/C_DOTA_ProjectileManager.h"
#include "../GameSystems/CGameEventManager.h"
#include "../Base/Memory.h"


namespace GameSystems {
	inline CDOTAGCClientSystem* GCClientSystem{};
	//GameRules is nullptr while not in a game
	//So we need to dereference it when we enter a match and reset to nullptr on leave
	inline CDOTAGameRules** GameRulesPtr{};
	inline CDOTAGameRules* GameRules{};

	inline C_DOTA_PlayerResource** PlayerResourcePtr{};
	inline C_DOTA_PlayerResource* PlayerResource{};

	inline CDOTAParticleManager** ParticleManagerPtr{};
	inline CDOTAParticleManager* ParticleManager{};

	inline C_DOTA_ProjectileManager** ProjectileManagerPtr{};
	inline C_DOTA_ProjectileManager* ProjectileManager{};

	inline CGameEventManager** GameEventManagerPtr{};
	inline CGameEventManager* GameEventManager{};

	void FindGameSystems();
	void LogGameSystems();
}