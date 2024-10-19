#include <Dota2SDK.h>

CBaseFileSystem* CBaseFileSystem::Get() {
	static CBaseFileSystem* inst = Memory::GetInterfaceBySubstr("filesystem_stdio.dll", "VFileSystem");
	return inst;
}

ISteamGameCoordinator* ISteamGameCoordinator::Get() {
	static ISteamGameCoordinator* inst = ISteamClient::Get()->GetISteamGenericInterface(
		Memory::GetExport("steam_api64.dll", "GetHSteamUser").Call<HSteamUser>(),
		Memory::GetExport("steam_api64.dll", "GetHSteamPipe").Call<HSteamPipe>(),
		"SteamGameCoordinator001"
	);
	return inst;
}

INetworkClientService* INetworkClientService::Get() {
	static INetworkClientService* inst = Memory::GetInterfaceBySubstr("engine2.dll", "NetworkClientService");
	return inst;
}

CSchemaSystem* CSchemaSystem::Get() {
	static CSchemaSystem* inst = Memory::GetInterfaceBySubstr("schemasystem.dll", "SchemaSystem_0");
	return inst;
}
CEngineServiceMgr* CEngineServiceMgr::Get() {
	static CEngineServiceMgr* inst = Memory::GetInterfaceBySubstr("engine2.dll", "EngineServiceMgr");
	return inst;
}
ISteamClient* ISteamClient::Get() {
	// There are multiple simultaneous versions of SteamClient
	// Current version can be found by searching for "SteamClient" in client.dll
	static ISteamClient* inst = Memory::GetInterface("steamclient64.dll", "SteamClient021");
	return inst;
}


CCVar* CCVar::Get() {
	static CCVar* inst = Memory::GetInterfaceBySubstr("tier0.dll", "VEngineCvar");
	return inst;
}

CSoundOpSystem* CSoundOpSystem::Get() {
	static CSoundOpSystem* inst = Memory::GetInterfaceBySubstr("soundsystem.dll", "SoundOpSystem");
	return inst;
}

CResourceSystem* CResourceSystem::Get() {
	static CResourceSystem* inst = Memory::GetInterfaceBySubstr("resourcesystem.dll", "ResourceSystem");
	return inst;
}

CGameUI* CGameUI::Get() {
	static CGameUI* inst = Memory::GetInterfaceBySubstr("client.dll", "GameUI0");
	return inst;
}

CGameEntitySystem* CGameEntitySystem::Get() {
	static CGameEntitySystem* inst =
		Address(
			CSource2Client::Get()->GetVFunc(VMI::CSource2Client::GetNetworkFieldChangeCallbackQueue)
		)
		.GetAbsoluteAddress(3, 7)
		.Dereference();
	return inst;
}

CFlattenedSerializers* CFlattenedSerializers::Get() {
	static CFlattenedSerializers* inst = Memory::GetInterfaceBySubstr("networksystem.dll", "FlattenedSerializers");
	return inst;
}

CSource2Client* CSource2Client::Get() {
	static CSource2Client* inst = Memory::GetInterfaceBySubstr("client.dll", "Source2Client0");
	return inst;
}

CGCClient* CGCClient::Get() {
	static CGCClient* inst = Memory::GetInterface("client.dll", "DOTA_CLIENT_GCCLIENT");
	return inst;
}

CUIEngineSource2* CUIEngineSource2::Get() {
	return CPanoramaUIEngine::Get()->GetSource2Engine();
}

CNetworkMessages* CNetworkMessages::Get() {
	static CNetworkMessages* inst = Memory::GetInterfaceBySubstr("networksystem.dll", "NetworkMessages");
	return inst;
}