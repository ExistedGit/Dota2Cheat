#include "GameSystems.h"
#include "Interfaces.h"
#include "Panorama.h"
#include <iomanip>
#include "../Tables.h"

Address FindStaticGameSystem(std::string_view name) {
	auto pFactory = IGameSystemFactory::Get();
	while (pFactory) {
		if (pFactory->m_szName && pFactory->m_szName == name)
			return pFactory->GetGameSystem();

		pFactory = pFactory->m_pNext;
	}

	return nullptr;
}

Address FindReallocatingGameSystem(std::string_view name) {
	auto pFactory = IGameSystemFactory::Get();
	while (pFactory) {
		if (pFactory->m_szName && pFactory->m_szName == name)
			return pFactory->GameSystem;

		pFactory = pFactory->m_pNext;
	}

	return nullptr;
}

void GameSystems::InitMinimapRenderer() {
	auto minimap = Panorama::DotaHud->FindChildByIdTraverse("minimap");
	while (!minimap) { minimap = Panorama::DotaHud->FindChildByIdTraverse("minimap"); };

	MinimapRenderer = minimap->GetPanel2D()->Member<CDOTAPanoramaMinimapRenderer*>(0x28);
}

void GameSystems::FindGameSystems() {

	// Found by xrefing this global in dylibs
	// look below the vfunc with xrefs "ehandle", "%d (s/n %d)", "[-1] -> empty", "m_flPoseParameter", "%s(%s)", "[%d %d] -> %s", "CStrongHandle", "CWeakHandle"
	Panorama::FindPanels();
	InitMinimapRenderer();

	LogI("[ GAME SYSTEMS ]");
	tables::PrettyPrint({
		{ "CDOTARichPresence", CRichPresence::Get()},
		{ "CDOTAGCClientSystem", CGCClientSys::Get() },
		{ "CDOTA_MinimapObjectManager", CMinimapObjMgr::Get() },
		{ "CDOTA_BinaryObjectSystem", CBinaryObjSys::Get() },
		{ "CDOTAInventoryManager", CInvMgr::Get() },
		{ "CGameParticleManagerSystem", CParticleMgrSys::Get() },
		{ "CDOTAPanoramaMinimapRenderer", MinimapRenderer },

		{ "C_DOTA_ProjectileManager*", CProjectileMgr::GetPtr() },
		{ "CRenderGameSystem*", CRenderGameSystem::GetPtr() },
		{ "CGameEventManager*", CEventMgr::GetPtr() },
		{ "C_DOTA_PlayerResource*", CPlayerResource::GetPtr() },
	});
}

IGameSystemFactory* IGameSystemFactory::Get() {
	// xrefs: "IGameSystem::InitAllSystems", "Game System %s is defined twice!\n" etc.
	static IGameSystemFactory* inst = *SignatureDB::FindSignature("g_pGameSystemFactory");
	return inst;
}
