#include "GameSystems.h"
#include "Interfaces.h"
#include "Panorama.h"

#define SET_VAR(var, data) var = (decltype(var))(data); \
if(var) \
	LogFD("{}: {}", #var, (void*)var); \
else \
	LogFE("{}: {}", #var, (void*)var);

void GameSystems::InitMinimapRenderer() {
	while (!Panorama::DotaHud->FindChildByIdTraverse("minimap")) {};
	auto minimap = Panorama::DotaHud->FindChildByIdTraverse("minimap");

	SET_VAR(MinimapRenderer, minimap->GetPanel2D()->Member<CDOTAPanoramaMinimapRenderer*>(0x28));
}

void GameSystems::FindGameSystems() {
	LogI("GAME SYSTEM POINTERS:");

	// xrefs: "CLoopModeGame::InitAllSystems()" "CBaseTempEntity::StaticInitTempEnts()"
	GameSystemFactory = *SignatureDB::FindSignature("g_pGameSystemFactory");

	// Found by xrefing this global in dylibs
	// look below the vfunc with xrefs "ehandle", "%d (s/n %d)", "[-1] -> empty", "m_flPoseParameter", "%s(%s)", "[%d %d] -> %s", "CStrongHandle", "CWeakHandle"
	PlayerResourcePtr = Address(Interfaces::Client->GetVFunc(VMI::CSource2Client::VoiceReliable)).Offset(4).GetAbsoluteAddress(3);

	// Also in CSource2Client::Init(), right after "g_GameEventManager.Init()":
	// mov rcx, [XXXXXXXXX]
	SET_VAR(GameEventManagerPtr, Address(Interfaces::Client->GetVFunc(VMI::CSource2Client::NotifyDisconnect))
		.Offset(0x3E)
		.GetAbsoluteAddress(3));

	SET_VAR(RichPresence, FindStaticGameSystem("CDOTARichPresence"));
	SET_VAR(GCClientSystem, FindStaticGameSystem("CDOTAGCClientSystem"));
	SET_VAR(MinimapObjManager, FindStaticGameSystem("CDOTA_MinimapObjectManager"));
	SET_VAR(BinaryObjectSystem, FindStaticGameSystem("CDOTA_BinaryObjectSystem"));
	SET_VAR(InventoryManager, FindStaticGameSystem("CDOTAInventoryManager"));
	SET_VAR(ParticleManagerSystem, FindStaticGameSystem("CGameParticleManagerSystem"));

	ProjectileManagerPtr = FindReallocatingGameSystem("C_DOTA_ProjectileManager");
	RenderGameSystemPtr = FindReallocatingGameSystem("RenderGameSystem");
}