#include "GameSystems.h"
#include "Interfaces.h"
#include "Panorama.h"

#define SET_VAR(var, data) var = (decltype(var))(data); \
if(var) \
	LogF(LP_DATA, "{}: {}", #var, (void*)var); \
else \
	LogF(LP_ERROR, "{}: {}", #var, (void*)var);

void GameSystems::InitMinimapRenderer() {
	while (!Panorama::DotaHud->FindChildByIdTraverse("minimap")) {};
	auto minimap = Panorama::DotaHud->FindChildByIdTraverse("minimap");

	SET_VAR(MinimapRenderer, minimap->GetPanel2D()->Member<CDOTAPanoramaMinimapRenderer*>(0x28));
}

Address GameSystems::FindGameSystem(const char* name) {
	auto pFactory = GameSystemFactory;
	while (pFactory) {
		if (pFactory->m_szName && !strcmp(pFactory->m_szName, name))
			return pFactory->GetGameSystem();

		pFactory = pFactory->m_pNext;
	}

	return nullptr;
}

void GameSystems::FindGameSystems() {
	Log(LP_INFO, "GAME SYSTEM POINTERS:");

	// xrefs: "CLoopModeGame::InitAllSystems()" "CBaseTempEntity::StaticInitTempEnts()"
	GameSystemFactory = *SignatureDB::FindSignature("g_pGameSystemFactory");

	// Found by xrefing this global in dylibs
	// look below the vfunc with xrefs "ehandle", "%d (s/n %d)", "[-1] -> empty", "m_flPoseParameter", "%s(%s)", "[%d %d] -> %s", "CStrongHandle", "CWeakHandle"
	PlayerResourcePtr = Address(Interfaces::Client->GetVFunc(VTableIndexes::CSource2Client::VoiceReliable)).Offset(4).GetAbsoluteAddress(3);

	// Also in CSource2Client::Init(), right after "g_GameEventManager.Init()":
	// mov rcx, [XXXXXXXXX]
	SET_VAR(GameEventManagerPtr, Address(Interfaces::Client->GetVFunc(VTableIndexes::CSource2Client::NotifyDisconnect))
		.Offset(0x3E)
		.GetAbsoluteAddress(3));

	SET_VAR(RichPresence, FindGameSystem("CDOTARichPresence"));
	SET_VAR(GCClientSystem, FindGameSystem("CDOTAGCClientSystem"));
	SET_VAR(MinimapObjManager, FindGameSystem("CDOTA_MinimapObjectManager"));
	SET_VAR(BinaryObjectSystem, FindGameSystem("CDOTA_BinaryObjectSystem"));
	SET_VAR(InventoryManager, FindGameSystem("CDOTAInventoryManager"));
	SET_VAR(ParticleManagerSystem, FindGameSystem("CGameParticleManagerSystem"));

	ProjectileManagerPtr = FindGameSystem("C_DOTA_ProjectileManager");
	RenderGameSystemPtr = FindGameSystem("RenderGameSystem");
}