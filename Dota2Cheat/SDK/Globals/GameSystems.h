#pragma once
#include "Interfaces.h"
#include "Context.h"
#include "SignatureDB.h"
#include "../Base/Memory.h"

#include "../GameSystems/C_DOTAGameRules.h"
#include "../GameSystems/C_DOTA_PlayerResource.h"
#include "../GameSystems/CDOTAParticleManager.h"
#include "../GameSystems/C_DOTA_ProjectileManager.h"
#include "../GameSystems/CGameEventManager.h"
#include "../GameSystems/CDOTARichPresence.h"
#include "../GameSystems/CDOTA_BinaryObjectSystem.h"
#include "../GameSystems/CDOTA_MinimapObjectManager.h"
#include "../GameSystems/CGameParticleManagerSystem.h"
#include "../GameSystems/CDOTA_PanoramaMinimapRenderer.h"
#include "../GameSystems/CRenderGameSystem.h"
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
	inline CDOTA_BinaryObjectSystem* BinaryObjectSystem{};
	inline Panorama::DotaHud* DotaHud{};
	void InitMinimapRenderer();

	inline CDOTAGCClientSystem* GCClientSystem{};
	inline VClass* InventoryManager{};
	inline CDOTARichPresence* RichPresence{};
	inline CDOTAGameRules* GameRules;
	inline CDOTA_MinimapObjectManager* MinimapObjManager{};
	inline CGameParticleManagerSystem* ParticleManagerSystem{};
	inline CDOTAParticleManager* ParticleManager{};
	// Ones that have a pointer to them that must be dereferenced when the game starts
	// so we need to store both the system and the ptr to it
#define REALLOCATING_SYSTEM(type, name) inline type* name {}; \
										inline type** name##Ptr {};

	REALLOCATING_SYSTEM(CGameEventManager, GameEventManager);
	REALLOCATING_SYSTEM(C_DOTA_ProjectileManager, ProjectileManager);
	REALLOCATING_SYSTEM(C_DOTA_PlayerResource, PlayerResource);
	REALLOCATING_SYSTEM(CRenderGameSystem, RenderGameSystem);

#undef REALLOCATING_SYSTEM

	template<typename T>
	T* FindStaticGameSystem(const char* name);
	template<typename T>
	T** FindReallocatingGameSystemPtr(const char* name);

	void FindGameSystems();
}