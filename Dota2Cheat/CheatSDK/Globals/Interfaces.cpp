#include "Interfaces.h"
#include <tuple>

template<typename T>
void InitInterface(T** var, std::string_view dllName, std::string_view interfaceName, std::optional<int> vmCount = std::nullopt) {
	T* instance = *var = Memory::GetInterfaceBySubstr<T>(dllName, interfaceName);
	if (!instance)
		return LogFE("{}: {}", interfaceName, (void*)instance);

	int countedVMs = CountVMs(instance);

	std::string vmInfo = " | VMs: " + std::to_string(countedVMs);

	if (vmCount.has_value() && countedVMs != vmCount) {
		vmInfo = std::format(" | VM count mismatch! Current: {}, Expected: {}", countedVMs, *vmCount);
		return LogFW("{}: {}{}", interfaceName, (void*)instance, vmInfo);
	}

	LogFD("{}: {}{}", interfaceName, (void*)instance, vmInfo);
}

void Interfaces::FindInterfaces() {
	Log(LP_NONE, "");

	LogI("[INTERFACES]");
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
	InitInterface(&NetworkMessages, "networksystem.dll", "NetworkMessages", 36);
	InitInterface(&FlattenedSerializers, "networksystem.dll", "FlattenedSerializers");
	// InitInterface(&NetworkSystem, "networksystem.dll", "NetworkSystem", 62);

	EntitySystem =
		*Address(Interfaces::Client->GetVFunc(VMI::CSource2Client::GetNetworkFieldChangeCallbackQueue))
		.GetAbsoluteAddress(3, 7);

	UIEngine = Panorama->Member<CUIEngineSource2*>(0x28);

	LogFD("UIEngine: {}", (void*)UIEngine);
	LogFD("EntitySystem: {}", (void*)EntitySystem);

	SteamGC = SteamClient->GetISteamGenericInterface(
		Memory::GetExport("steam_api64.dll", "GetHSteamPipe")(),
		Memory::GetExport("steam_api64.dll", "GetHSteamUser")(),
		"SteamGameCoordinator001"
	);

	Log(LP_NONE, "");
}
