#include "Interfaces.h"
#include <tuple>

void InitInterface(auto** var, const char* dllName, const char* interfaceName, std::optional<int> vmCount = std::nullopt) {
	auto instance = *(void**)var = Memory::GetInterface(dllName, interfaceName);
	if (!instance)
		return LogF(LP_ERROR, "{}: {}", interfaceName, instance);

	int countedVMs = CountVMs(instance);

	LogPrefix prefix = LP_DATA;
	std::string vmInfo = " | VMs: " + std::to_string(countedVMs);

	if (vmCount.has_value() && countedVMs != vmCount) {
		vmInfo = std::format(" | VM count mismatch! Current: {}, Required: {}", countedVMs, *vmCount);
		prefix = LP_WARNING;
	}

	LogF(prefix, "{}: {}{}", interfaceName, instance, vmInfo);
}

void Interfaces::FindInterfaces() {
	Log(LP_NONE, "");

	Log(LP_INFO, "[INTERFACES]");
	InitInterface(&Engine, "engine2.dll", "Source2EngineToClient001", 177);
	InitInterface(&Client, "client.dll", "Source2Client002");
	InitInterface(&CVar, "tier0.dll", "VEngineCvar007", 42);
	InitInterface(&ResourceSystem, "resourcesystem.dll", "ResourceSystem013", 78);
	InitInterface(&SteamClient, "steamclient64.dll", "SteamClient017");
	InitInterface(&FileSystem, "filesystem_stdio.dll", "VFileSystem017", 136);

	InitInterface(&Panorama, "panorama.dll", "PanoramaUIEngine001");

	InitInterface(&GCClient, "client.dll", "DOTA_CLIENT_GCCLIENT");
	InitInterface(&GameUI, "client.dll", "LegacyGameUI001");
	InitInterface(&Schema, "schemasystem.dll", "SchemaSystem_001", 38);
	InitInterface(&ParticleMgrSystem, "particles.dll", "ParticleSystemMgr003");
	InitInterface(&InputService, "engine2.dll", "InputService_001", 64);
	InitInterface(&NetworkClientService, "engine2.dll", "NetworkClientService_001");
	InitInterface(&NetworkSystem, "networksystem.dll", "NetworkSystemVersion001", 62);
	InitInterface(&NetworkMessages, "networksystem.dll", "NetworkMessagesVersion001", 36);
	EntitySystem = *Address(Interfaces::Client->GetVFunc(25).ptr).GetAbsoluteAddress<CGameEntitySystem**>(3, 7);
	UIEngine = Panorama->Member<Panorama::CUIEngineSource2*>(0x28);

	LogF(LP_DATA, "UIEngine: {}", (void*)UIEngine);
	LogF(LP_DATA, "EntitySystem: {}", (void*)EntitySystem);

	SteamGC = SteamClient->GetISteamGenericInterface(
		Memory::GetExport("steam_api64.dll", "GetHSteamPipe")(),
		Memory::GetExport("steam_api64.dll", "GetHSteamUser")(),
		"SteamGameCoordinator001"
	);

	Log(LP_NONE, "");
}
