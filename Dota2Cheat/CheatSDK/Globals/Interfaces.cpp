#include "Interfaces.h"
#include <tuple>

void InitInterface(auto** var, const char* dllName, const char* interfaceName, std::optional<int> vmCount = std::nullopt) {
	auto instance = *(void**)var = Memory::GetInterfaceBySubstr(dllName, interfaceName);
	if (!instance)
		return LogF(LP_ERROR, "{}: {}", interfaceName, instance);

	int countedVMs = CountVMs(instance);

	LogPrefix prefix = LP_DATA;
	std::string vmInfo = " | VMs: " + std::to_string(countedVMs);

	if (vmCount.has_value() && countedVMs != vmCount) {
		vmInfo = std::format(" | VM count mismatch! Current: {}, Expected: {}", countedVMs, *vmCount);
		prefix = LP_WARNING;
	}

	LogF(prefix, "{}: {}{}", interfaceName, instance, vmInfo);
}

void Interfaces::FindInterfaces() {
	Log(LP_NONE, "");

	Log(LP_INFO, "[INTERFACES]");
	InitInterface(&Engine, "engine2.dll", "Source2EngineToClient0", 177);
	InitInterface(&Client, "client.dll", "Source2Client0");
	InitInterface(&CVar, "tier0.dll", "VEngineCvar", 42);
	InitInterface(&ResourceSystem, "resourcesystem.dll", "ResourceSystem", 78);
	InitInterface(&SteamClient, "steamclient64.dll", "SteamClient017");
	InitInterface(&FileSystem, "filesystem_stdio.dll", "VFileSystem", 136);

	InitInterface(&Panorama, "panorama.dll", "PanoramaUIEngine");

	InitInterface(&GCClient, "client.dll", "DOTA_CLIENT_GCCLIENT");
	InitInterface(&GameUI, "client.dll", "GameUI0");
	InitInterface(&Schema, "schemasystem.dll", "SchemaSystem", 38);
	InitInterface(&ParticleMgrSystem, "particles.dll", "ParticleSystemMgr");
	InitInterface(&SoundOpSystem, "soundsystem.dll", "SoundOpSystem");
	InitInterface(&InputService, "engine2.dll", "InputService", 64);
	InitInterface(&NetworkClientService, "engine2.dll", "NetworkClientService");
	// InitInterface(&NetworkSystem, "networksystem.dll", "NetworkSystem", 62);
	InitInterface(&NetworkMessages, "networksystem.dll", "NetworkMessages", 36);
	InitInterface(&FlattenedSerializers, "networksystem.dll", "FlattenedSerializers");

	EntitySystem =
		*Address(Interfaces::Client->GetVFunc(25))
		.GetAbsoluteAddress(3, 7);
	UIEngine = Panorama->Member<CUIEngineSource2*>(0x28);

	LogF(LP_DATA, "UIEngine: {}", (void*)UIEngine);
	LogF(LP_DATA, "EntitySystem: {}", (void*)EntitySystem);

	SteamGC = SteamClient->GetISteamGenericInterface(
		Memory::GetExport("steam_api64.dll", "GetHSteamPipe")(),
		Memory::GetExport("steam_api64.dll", "GetHSteamUser")(),
		"SteamGameCoordinator001"
	);

	Log(LP_NONE, "");
}
