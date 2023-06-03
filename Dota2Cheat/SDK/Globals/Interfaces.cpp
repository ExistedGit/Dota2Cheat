#include "Interfaces.h"
#include <tuple>
void* Interfaces::GetInterface(const char* dllName, const char* interfaceName) {
	auto CreateInterface = Memory::GetExport(dllName, "CreateInterface");
	void* result = CreateInterface(interfaceName, nullptr);
	return result;
}

void InitInterface(auto** var, const char* dllName, const char* interfaceName, std::optional<int> vmCount = std::nullopt) {
	auto instance = *(void**)var = Interfaces::GetInterface(dllName, interfaceName);
	int countedVMs = CountVMs(instance);

	LogPrefix prefix = LP_DATA;
	std::string vmInfo = " | VMs: " + std::to_string(countedVMs);
	if (vmCount.has_value() && countedVMs != vmCount) {
		vmInfo = std::format(" | VM count mismatch! Current: {}, Required: {}", countedVMs, *vmCount);
		prefix = LP_WARNING;
	}

	LogF(prefix, "{}/{}: {}{}", dllName, interfaceName, instance, vmInfo);
}


void Interfaces::FindInterfaces() {
	Log(LP_INFO, "[INTERFACES]");
	InitInterface(&Engine, "engine2.dll", "Source2EngineToClient001");
	InitInterface(&Engine, "engine2.dll", "Source2EngineToClient001", 177);
	InitInterface(&Client, "client.dll", "Source2Client002");
	InitInterface(&CVar, "tier0.dll", "VEngineCvar007", 42);
	InitInterface(&ResourceSystem, "resourcesystem.dll", "ResourceSystem013", 78);
	InitInterface(&SteamClient, "steamclient64.dll", "SteamClient017");
	InitInterface(&FileSystem, "filesystem_stdio.dll", "VFileSystem017", 136);

	InitInterface(&Panorama, "panorama.dll", "PanoramaUIEngine001");

	InitInterface(&GCClient, "client.dll", "DOTA_CLIENT_GCCLIENT");
	InitInterface(&Schema, "schemasystem.dll", "SchemaSystem_001", 38);
	InitInterface(&ParticleMgrSystem, "particles.dll", "ParticleSystemMgr003");
	InitInterface(&InputService, "engine2.dll", "InputService_001", 64);
	InitInterface(&NetworkSystem, "networksystem.dll", "NetworkSystemVersion001", 62);
	InitInterface(&NetworkMessages, "networksystem.dll", "NetworkMessagesVersion001", 36);
	EntitySystem = *Address(Interfaces::Client->GetVFunc(25).ptr).GetAbsoluteAddress<CGameEntitySystem**>(3, 7);
	LogF(LP_DATA, "EntitySystem: {}", (void*)EntitySystem);
	UIEngine = Panorama->Member<Panorama::CUIEngineSource2*>(0x28);
}
