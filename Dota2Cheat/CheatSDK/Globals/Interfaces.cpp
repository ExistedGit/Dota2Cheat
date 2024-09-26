#include "Interfaces.h"
#include <tuple>
#include <iomanip>

template<typename T>
void InitInterface(T*& var, std::string_view dllName, std::string_view interfaceName) {
	var = Memory::GetInterfaceBySubstr(dllName, interfaceName);
}

void Interfaces::FindInterfaces() {
	LogI("[ INTERFACES ]");

	InitInterface(Client, "client.dll", "Source2Client0");
	InitInterface(GCClient, "client.dll", "DOTA_CLIENT_GCCLIENT");
	InitInterface(GameUI, "client.dll", "GameUI0");

	InitInterface(Engine, "engine2.dll", "Source2EngineToClient0");
	InitInterface(InputService, "engine2.dll", "InputService");
	InitInterface(NetworkClientService, "engine2.dll", "NetworkClientService");

	InitInterface(NetworkMessages, "networksystem.dll", "NetworkMessages");
	InitInterface(FlattenedSerializers, "networksystem.dll", "FlattenedSerializers");

	InitInterface(CVar, "tier0.dll", "VEngineCvar");
	InitInterface(ResourceSystem, "resourcesystem.dll", "ResourceSystem");

	SteamClient = Memory::GetInterface("steamclient64.dll", "SteamClient021");
	//SteamClient = Memory::GetExport("steam_api64.dll", "SteamInternal_CreateInterface").Call<ISteamClient*>("SteamClient021");

	InitInterface(FileSystem, "filesystem_stdio.dll", "VFileSystem");
	InitInterface(Panorama, "panorama.dll", "PanoramaUIEngine");
	InitInterface(Schema, "schemasystem.dll", "SchemaSystem");
	InitInterface(ParticleMgrSystem, "particles.dll", "ParticleSystemMgr");
	InitInterface(SoundOpSystem, "soundsystem.dll", "SoundOpSystem");

	// InitInterface(&NetworkSystem, "networksystem.dll", "NetworkSystem");

	EntitySystem =
		*Address(Interfaces::Client->GetVFunc(VMI::CSource2Client::GetNetworkFieldChangeCallbackQueue))
		.GetAbsoluteAddress(3, 7);

	UIEngine = Panorama->Member<CUIEngineSource2*>(0x28);

	SteamGC = SteamClient->GetISteamGenericInterface(
		Memory::GetExport("steam_api64.dll", "GetHSteamUser").Call<HSteamUser>(),
		Memory::GetExport("steam_api64.dll", "GetHSteamPipe").Call<HSteamPipe>(),
		"SteamGameCoordinator001"
	);

	struct LogPtrBinding {
		std::string name;
		void* value;

		LogPtrBinding(std::string name, void* value) : value(value) {
			this->name = std::move(name);
		}
	};

	std::vector<LogPtrBinding> loggingScheme = {
		{ "CEngineClient", Engine },
		{ "CSource2Client", Client },
		{ "CCVar", CVar },
		{ "CResourceSystem", ResourceSystem },
		{ "CBaseFileSystem", FileSystem },
		{ "CGameUI", GameUI },
		{ "CSoundOpSystem", SoundOpSystem},
		{ "CInputService", InputService},
		{ "CDOTAParticleSystemMgr", ParticleMgrSystem},
		{ "CGameEntitySystem", EntitySystem},

		{ "CUIEngine", Panorama},
		{ "CUIEngineSource2", UIEngine},

		{ "INetworkClientService", NetworkClientService},
		{ "CNetworkMessages", NetworkMessages },
		{ "CFlattenedSerializers", FlattenedSerializers },

		{ "ISteamClient", SteamClient },
		{ "ISteamGameCoordinator", SteamGC },
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
