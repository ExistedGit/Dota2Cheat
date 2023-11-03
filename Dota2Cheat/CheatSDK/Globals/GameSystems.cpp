#include "GameSystems.h"
#include "Interfaces.h"
#include "Panorama.h"

#define SET_VAR(var, data) var = (decltype(var))(data); \
if(var) \
	LogF(LP_DATA, "{}: {}", #var, (void*)var); \
else \
	LogF(LP_ERROR, "{}: {}", #var, (void*)var);

void GameSystems::InitMinimapRenderer() {
	while (!Panorama::DotaHud->FindChildWithIdTraverse("minimap")) {};
	auto minimap = Panorama::DotaHud->FindChildWithIdTraverse("minimap");

	SET_VAR(MinimapRenderer, minimap->GetPanel2D()->Member<CDOTAPanoramaMinimapRenderer*>(0x28));
}
template<typename T>
T* GameSystems::FindStaticGameSystem(const char* name) {
	auto pFactory = GameSystemFactory;
	while (pFactory) {
		if (pFactory->m_szName && !strcmp(pFactory->m_szName, name))
			return (T*)pFactory->GetGameSystem();

		pFactory = pFactory->m_pNextFactory;
	}

	return nullptr;
}

template<typename T>
T** GameSystems::FindReallocatingGameSystemPtr(const char* name) {
	auto pFactory = GameSystemFactory;
	while (pFactory) {
		if (pFactory->m_szName && !strcmp(pFactory->m_szName, name))
			return (T**)pFactory->GameSystem;

		pFactory = pFactory->m_pNextFactory;
	}

	return nullptr;
}

void GameSystems::FindGameSystems() {
	Log(LP_INFO, "GAME SYSTEM POINTERS:");

	// xrefs: "CLoopModeGame::InitAllSystems()" "CBaseTempEntity::StaticInitTempEnts()"
	GameSystemFactory = *Memory::Scan("E8 ? ? ? ? 84 C0 74 D3 48 8D 0D", "client.dll")
		.GetAbsoluteAddress(1)
		.Offset(0xE)
		.GetAbsoluteAddress(3);

	// Found by xrefing this global in dylibs
	// look below the vfunc with xrefs "ehandle", "%d (s/n %d)", "[-1] -> empty", "m_flPoseParameter", "%s(%s)", "[%d %d] -> %s", "CStrongHandle", "CWeakHandle"
	PlayerResourcePtr = Address(Interfaces::Client->GetVFunc(VTableIndexes::Source2Client::VoiceReliable)).Offset(4).GetAbsoluteAddress(3);

	// Also in Source2Client::Init(), right after "g_GameEventManager.Init()":
	// mov rcx, [XXXXXXXXX]
	SET_VAR(GameEventManagerPtr, Address(Interfaces::Client->GetVFunc(VTableIndexes::Source2Client::NotifyDisconnect))
		.Offset(0x3E)
		.GetAbsoluteAddress(3));

	RichPresence = FindStaticGameSystem<CDOTARichPresence>("CDOTARichPresence");
	GCClientSystem = FindStaticGameSystem<CDOTAGCClientSystem>("CDOTAGCClientSystem");
	SET_VAR(MinimapObjManager, FindStaticGameSystem<CDOTA_MinimapObjectManager>("CDOTA_MinimapObjectManager"));
	SET_VAR(BinaryObjectSystem, FindStaticGameSystem<CDOTA_BinaryObjectSystem>("CDOTA_BinaryObjectSystem"));
	SET_VAR(InventoryManager, FindStaticGameSystem<VClass>("CDOTAInventoryManager"));
	SET_VAR(ParticleManagerSystem, FindStaticGameSystem<CGameParticleManagerSystem>("CGameParticleManagerSystem"));
	ProjectileManagerPtr = FindReallocatingGameSystemPtr<C_DOTA_ProjectileManager>("C_DOTA_ProjectileManager");
	RenderGameSystemPtr = FindReallocatingGameSystemPtr<CRenderGameSystem>("RenderGameSystem");
}