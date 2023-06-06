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
	struct IGameSystemFactory : public VClass {
		IGameSystemFactory* m_pNextFactory;
		const char* m_szName;
		void** GameSystem;

		VGETTER(void*, GetGameSystem, 9);
	};
	inline IGameSystemFactory* GameSystemFactory{};

	inline CDOTAPanoramaMinimapRenderer* MinimapRenderer{};
	inline Panorama::DotaHud* DotaHud{};
	void InitMinimapRenderer();

	inline CGameUI* GameUI{};
	inline CDOTAGCClientSystem* GCClientSystem{};
	inline CDOTARichPresence* RichPresence{};

	// Ones that have a pointer to them that must be dereferenced when the game starts
	// so we need to store both the system and the ptr to it
#define REALLOCATING_SYSTEM(type, name) inline type* name; \
										inline type** name##Ptr;

	REALLOCATING_SYSTEM(CGlobalVars, GlobalVars);
	REALLOCATING_SYSTEM(CGameEventManager, GameEventManager);
	REALLOCATING_SYSTEM(C_DOTA_ProjectileManager, ProjectileManager);
	REALLOCATING_SYSTEM(CDOTAParticleManager, ParticleManager);
	REALLOCATING_SYSTEM(C_DOTA_PlayerResource, PlayerResource);
	REALLOCATING_SYSTEM(CDOTAGameRules, GameRules);

#undef REALLOCATING_SYSTEM

	static inline std::map<std::string, void**> NamedSystems{
		SIGMAP_ENTRY(PlayerResourcePtr),
		SIGMAP_ENTRY(ParticleManagerPtr),
		SIGMAP_ENTRY(GameUI),
	};
	template<typename T>
	T* FindStaticGameSystem(const char* name);
	template<typename T>
	T** FindReallocatingGameSystemPtr(const char* name);

	void FindGameSystems();
}