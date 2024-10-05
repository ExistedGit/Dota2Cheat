#pragma once
#include "SignatureDB.h"

#include "Base/Memory.h"

#include "GameSystems/C_DOTA_PlayerResource.h"
#include "GameSystems/C_DOTA_ProjectileManager.h"
#include "GameSystems/CGameEventManager.h"
#include "GameSystems/CDOTARichPresence.h"
#include "GameSystems/CDOTA_MinimapObjectManager.h"
#include "GameSystems/CDOTA_PanoramaMinimapRenderer.h"
#include "GameSystems/CGlobalVars.h"
#include "GameSystems/CDOTAParticleManager.h"
#include "GameSystems/CDOTA_BinaryObjectSystem.h"
#include "GameSystems/C_DOTAGameRules.h"
#include "GameSystems/CGameParticleManagerSystem.h"
#include "GameSystems/CRenderGameSystem.h"
#include "GameSystems/CDOTAInventoryManager.h"
#include "GameSystems/CDOTAGCClientSystem.h"

#include "../Aliases.h"

struct IGameSystemFactory : public VClass {
	IGameSystemFactory* m_pNext;
	const char* m_szName;
	void** GameSystem;

	VGETTER(void*, GetGameSystem, 9);

	static IGameSystemFactory* Get();
};

namespace GameSystems {
	void FindGameSystems();
}