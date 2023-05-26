#pragma once
#include "Interfaces.h"
#include "Context.h"
#include "SignatureDB.h"
#include "../Base/Memory.h"

#include "../GameSystems/C_DOTAGameRules.h"
#include "../GameSystems/CGlobalVars.h"
#include "../GameSystems/C_DOTA_PlayerResource.h"
#include "../GameSystems/CDOTAParticleManager.h"
#include "../GameSystems/C_DOTA_ProjectileManager.h"
#include "../GameSystems/CGameEventManager.h"
#include "../GameSystems/CDOTARichPresence.h"
#include "../GameSystems/CGameUI.h"
#include "../GameSystems/CDOTA_PanoramaMinimapRenderer.h"
#include "../Interfaces/Panorama.h"

namespace GameSystems {
	inline CDOTAPanoramaMinimapRenderer* MinimapRenderer{};
	inline Panorama::DotaHud* DotaHud{};
	void InitMinimapRenderer();

	inline CGlobalVars** GlobalVarsPtr{};
	inline CGlobalVars* GlobalVars{};
	inline CGameUI* GameUI{};
	inline CDOTAGCClientSystem* GCClientSystem{};
	//GameRules is nullptr while not in a game
	//So we need to dereference it when we enter a match and reset to nullptr on leave
	inline CDOTAGameRules** GameRulesPtr{};
	inline CDOTAGameRules* GameRules{};

	inline C_DOTA_PlayerResource** PlayerResourcePtr{};
	inline C_DOTA_PlayerResource* PlayerResource{};

	inline CDOTAParticleManager** ParticleManagerPtr{};
	inline CDOTAParticleManager* ParticleManager{};

	inline C_DOTA_ProjectileManager* ProjectileManager{};

	inline CDOTARichPresence* RichPresence{};

	inline CGameEventManager** GameEventManagerPtr{};
	inline CGameEventManager* GameEventManager{};


	static inline std::map<std::string, void**> NamedSystems{
		SIGMAP_ENTRY(PlayerResourcePtr),
		SIGMAP_ENTRY(ParticleManagerPtr),
		SIGMAP_ENTRY(GameUI),
	};
	void GetGameSystemViaFactory(const char* name, void** val);
	void FindGameSystems();
}