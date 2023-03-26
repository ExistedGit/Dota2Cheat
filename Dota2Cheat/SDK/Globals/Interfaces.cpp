#include "Interfaces.h"

void Interfaces::FindInterfaces() {
	std::cout << "[INTERFACES]\n";
	Engine = GetInterface<CEngineClient>("engine2.dll", "Source2EngineToClient001");
	Client = GetInterface<VClass>("client.dll", "Source2Client002", 151);
	CVar = GetInterface<CVarSystem>("tier0.dll", "VEngineCvar007");
	ResourceSystem = GetInterface<CResourceSystem>("resourcesystem.dll", "ResourceSystem013", 81);
	EntitySystem = *Address(Interfaces::Client->GetVFunc(25).ptr).GetAbsoluteAddress<CGameEntitySystem**>(3, 7);

	FileSystem = GetInterface<CBaseFileSystem>("filesystem_stdio.dll", "VFileSystem017");

	Panorama = GetInterface<VClass>("panorama.dll", "PanoramaUIEngine001");
	UIEngine = Panorama->Member<Panorama::CUIEngineSource2*>(0x28);

	GCClient = GetInterface<CGCClient>("client.dll", "DOTA_CLIENT_GCCLIENT");
	Schema = GetInterface<VClass>("schemasystem.dll", "SchemaSystem_001");
	ParticleMgrSystem = GetInterface<VClass>("particles.dll", "ParticleSystemMgr003");
	InputService = GetInterface<CInputService>("engine2.dll", "InputService_001");
	NetworkSystem = GetInterface<void>("networksystem.dll", "NetworkSystemVersion001");
	NetworkMessages = GetInterface<CNetworkMessages>("networksystem.dll", "NetworkMessagesVersion001");
}
