#pragma once
#include "SignatureDB.h"

#include "../../SDK/Base/Memory.h"

#include "../../SDK/GameSystems/C_DOTAGameRules.h"
#include "../../SDK/GameSystems/C_DOTA_PlayerResource.h"
#include "../../SDK/GameSystems/CDOTAParticleManager.h"
#include "../../SDK/GameSystems/C_DOTA_ProjectileManager.h"
#include "../../SDK/GameSystems/CGameEventManager.h"
#include "../../SDK/GameSystems/CDOTARichPresence.h"
#include "../../SDK/GameSystems/CDOTA_BinaryObjectSystem.h"
#include "../../SDK/GameSystems/CDOTA_MinimapObjectManager.h"
#include "../../SDK/GameSystems/CGameParticleManagerSystem.h"
#include "../../SDK/GameSystems/CDOTA_PanoramaMinimapRenderer.h"
#include "../../SDK/GameSystems/CRenderGameSystem.h"

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