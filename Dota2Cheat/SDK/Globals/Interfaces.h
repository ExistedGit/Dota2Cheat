#pragma once

#include "../Interfaces/CGameEntitySystem.h"
#include "../Interfaces/IEngineClient.h"
#include "../Interfaces/CInputService.h"
#include "../Interfaces/CBaseFileSystem.h"
#include "../Interfaces/Network/CNetworkMessages.h"
#include "../Interfaces/Network/CNetworkGameClient.h"
#include "../Interfaces/Network/INetworkClientService.h"
#include "../Interfaces/GC/CGCClient.h"
#include "../Interfaces/CVarSystem.h"
#include "../Interfaces/Panorama.h"
#include "../Interfaces/CResourceSystem.h"
#include "../Interfaces/CSoundOpSystem.h"
#include "../Interfaces/ISteamClient.h"
#include "../Interfaces/ISteamGameCoordinator.h"
#include "../Interfaces/CGameUI.h"

#include "../Base/Memory.h"
#include "../Base/Address.h"
#include "../Base/VMT.h"
#include "../Base/Logging.h"

#include <format>

namespace Interfaces {
	inline CVarSystem* CVar{};
	inline CEngineClient* Engine{};
	inline CResourceSystem* ResourceSystem{};
	inline VClass* Panorama{};
	inline Panorama::CUIEngineSource2* UIEngine{};
	inline VClass* Client{}; // CSource2Client 
	inline CGameEntitySystem* EntitySystem{};
	inline ISteamClient* SteamClient{};
	inline ISteamGameCoordinator* SteamGC{};
	inline CSoundOpSystem* SoundOpSystem{};
	inline CGameUI* GameUI{};
	inline CGCClient* GCClient{};

	inline INetworkClientService* NetworkClientService{};
	inline CNetworkMessages* NetworkMessages{};

	inline CInputService* InputService{};
	inline VClass* Schema{};
	inline VClass* ParticleMgrSystem{};
	inline CBaseFileSystem* FileSystem{};
	inline void* NetworkSystem;

	void FindInterfaces();
}