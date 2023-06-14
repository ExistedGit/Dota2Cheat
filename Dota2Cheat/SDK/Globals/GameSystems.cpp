#include "GameSystems.h"
#include "Interfaces.h"
#include <consthash/cityhash32.hxx>
#include <cityhash/city.h>
#define SET_VAR(var, data) var = (decltype(var))(data); \
if(var) \
	LogF(LP_DATA, "{}: {}", #var, (void*)var); \
else \
	LogF(LP_ERROR, "{}: {}", #var, (void*)var);

void GameSystems::InitMinimapRenderer() {
	while (!DotaHud->FindChildWithIdTraverse("minimap")) {};
	auto minimap = DotaHud->FindChildWithIdTraverse("minimap");

	SET_VAR(MinimapRenderer, minimap->GetPanel2D()->Member<CDOTAPanoramaMinimapRenderer*>(0x20));
}
template<typename T>
T* GameSystems::FindStaticGameSystem(const char* name) {
	auto pFactory = GameSystemFactory;
	while (pFactory) {
		if (pFactory->m_szName && !strcmp(pFactory->m_szName, name))
			return (T*)pFactory->GetGameSystem();
		
		pFactory = pFactory->m_pNextFactory;
	}
}

template<typename T>
T** GameSystems::FindReallocatingGameSystemPtr(const char* name) {
	auto pFactory = GameSystemFactory;
	while (pFactory) {
		if (pFactory->m_szName && !strcmp(pFactory->m_szName, name))
			return (T**)pFactory->GameSystem;

		pFactory = pFactory->m_pNextFactory;
	}
}

void GameSystems::FindGameSystems() {
	Log(LP_INFO, "GAME SYSTEM POINTERS:");

	GameSystemFactory = *Memory::Scan("E8 ? ? ? ? 84 C0 74 D3 48 8D 0D", "client.dll")
		.GetAbsoluteAddress(1)
		.Offset(0xE)
		.GetAbsoluteAddress<IGameSystemFactory**>(3);

	SignatureDB::ParseSignatures(NamedSystems);

	// Also in Source2Client::Init(), right after "g_GameEventManager.Init()":
	// mov rcx, [XXXXXXXXX]
	SET_VAR(GameEventManagerPtr, Address(Interfaces::Client->GetVFunc(13).ptr)
		.Offset(0x3E)
		.GetAbsoluteAddress(3));
	// Source2Client::SetGlobalVars()
	SET_VAR(GlobalVarsPtr, Address(Interfaces::Client->GetVFunc(11).ptr)
		.GetAbsoluteAddress<CGlobalVars**>(3));

	RichPresence = FindStaticGameSystem<CDOTARichPresence>("CDOTARichPresence");
	GCClientSystem = FindStaticGameSystem<CDOTAGCClientSystem>("CDOTAGCClientSystem");
	SET_VAR(MinimapObjManager, GameSystems::FindStaticGameSystem<CDOTA_MinimapObjectManager>("CDOTA_MinimapObjectManager"));
	ProjectileManagerPtr = FindReallocatingGameSystemPtr<C_DOTA_ProjectileManager>("C_DOTA_ProjectileManager");

	//xref: "activategameui", first lea rax, [XXXXXXXXX]
	//console command ^
	//SET_VAR(GameUI, Memory::Scan("E8 ? ? ? ? 48 8B C8 E8 ? ? ? ? 48 8B 3D", "client.dll")
	//	.GetAbsoluteAddress(1)
	//	.GetAbsoluteAddress(3));

	// SET_VAR(GameRulesPtr, Address(CDOTAGameRules::GetGameTimeFunc).Offset(0xF).GetAbsoluteAddress(3, 7));

	// xref: "No player resource\n"
	//SET_VAR(PlayerResourcePtr, Memory::Scan("40 57 48 83 EC 70 48 8B 3D", "client.dll")
	//	.Offset(6)
	//	.GetAbsoluteAddress(3, 7));

	// xref: "flOffset"
	// lea rcx, [XXXXXXXXX] above it is GetParticleManager()
	//SET_VAR(ParticleManagerPtr, Memory::Scan("E8 ? ? ? ? 8B 14 9F", "client.dll")
	//	.GetAbsoluteAddress(1, 5)
	//	.GetAbsoluteAddress(3, 7));


}