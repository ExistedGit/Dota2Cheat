#include "GameSystems.h"
#include "Interfaces.h"
#include "Panorama.h"
#include <iomanip>

Address FindStaticGameSystem(std::string_view name) {
	auto pFactory = IGameSystemFactory::GetInstance();
	while (pFactory) {
		if (pFactory->m_szName && pFactory->m_szName == name)
			return pFactory->GetGameSystem();

		pFactory = pFactory->m_pNext;
	}

	return nullptr;
}

Address FindReallocatingGameSystem(std::string_view name) {
	auto pFactory = IGameSystemFactory::GetInstance();
	while (pFactory) {
		if (pFactory->m_szName && pFactory->m_szName == name)
			return pFactory->GameSystem;

		pFactory = pFactory->m_pNext;
	}

	return nullptr;
}

void GameSystems::InitMinimapRenderer() {
	while (!Panorama::DotaHud->FindChildByIdTraverse("minimap")) {};
	auto minimap = Panorama::DotaHud->FindChildByIdTraverse("minimap");

	MinimapRenderer = minimap->GetPanel2D()->Member<CDOTAPanoramaMinimapRenderer*>(0x28);
}

void GameSystems::FindGameSystems() {
	LogI("[ GAME SYSTEMS ]");

	// Found by xrefing this global in dylibs
	// look below the vfunc with xrefs "ehandle", "%d (s/n %d)", "[-1] -> empty", "m_flPoseParameter", "%s(%s)", "[%d %d] -> %s", "CStrongHandle", "CWeakHandle"
	PlayerResourcePtr = Address(Interfaces::Client->GetVFunc(VMI::CSource2Client::VoiceReliable)).Offset(4).GetAbsoluteAddress(3);

	// Also in CSource2Client::Init(), right after "g_GameEventManager.Init()":
	// mov rcx, [XXXXXXXXX]
	GameEventManagerPtr =
		Address(Interfaces::Client->GetVFunc(VMI::CSource2Client::NotifyDisconnect))
		.Offset(0x3E)
		.GetAbsoluteAddress(3);

	RichPresence = FindStaticGameSystem("CDOTARichPresence");
	GCClientSystem = FindStaticGameSystem("CDOTAGCClientSystem");
	MinimapObjManager = FindStaticGameSystem("CDOTA_MinimapObjectManager");
	BinaryObjectSystem = FindStaticGameSystem("CDOTA_BinaryObjectSystem");
	InventoryManager = FindStaticGameSystem("CDOTAInventoryManager");
	ParticleManagerSystem = FindStaticGameSystem("CGameParticleManagerSystem");

	ProjectileManagerPtr = FindReallocatingGameSystem("C_DOTA_ProjectileManager");
	//RenderGameSystemPtr = FindReallocatingGameSystem("RenderGameSystem");

	InitMinimapRenderer();

	struct LogPtrBinding {
		std::string name;
		void* value;

		LogPtrBinding(std::string name, void* value, bool reallocating = false) : value(value) {
			this->name = std::move(name);

			if (reallocating) this->name += "*";
		}
	};

	std::vector<LogPtrBinding> loggingScheme = {
		{ "CDOTARichPresence", RichPresence },
		{ "CDOTAGCClientSystem", GCClientSystem },
		{ "CDOTA_MinimapObjectManager", GCClientSystem },
		{ "CDOTA_BinaryObjectSystem", GCClientSystem },
		{ "CDOTAInventoryManager", GCClientSystem },
		{ "CGameParticleManagerSystem", GCClientSystem },
		{ "CDOTAPanoramaMinimapRenderer", MinimapRenderer },

		{ "C_DOTA_ProjectileManager", ProjectileManagerPtr, true},
		//{ "CRenderGameSystem", RenderGameSystemPtr, true },
		{ "CGameEventManager", GameEventManagerPtr, true },
		{ "C_DOTA_PlayerResource", PlayerResourcePtr, true },
	};

	size_t padding = 0;
	{
		auto longestNameElem = std::max_element(loggingScheme.begin(), loggingScheme.end(),
			[](const LogPtrBinding& a, const LogPtrBinding& b) {
				return a.name.length() < b.name.length();
			});
		if (longestNameElem != loggingScheme.end()) padding = longestNameElem->name.size();
	}

	{
		std::lock_guard lock(mLogging);
		for (const auto& binding : loggingScheme) {
			LogPrefix prefix = binding.value ? LP_DATA : LP_ERROR;
			SetConsoleColor();
			SetLogColor(prefix);

			std::cout << std::left << std::setw(padding) << binding.name << " = " << std::hex << std::uppercase << binding.value << std::endl;
		}
	}
}

IGameSystemFactory* IGameSystemFactory::GetInstance() {
	// xrefs: "IGameSystem::InitAllSystems", "Game System %s is defined twice!\n" etc.
	static IGameSystemFactory* inst = *SignatureDB::FindSignature("g_pGameSystemFactory");
	return inst;
}
