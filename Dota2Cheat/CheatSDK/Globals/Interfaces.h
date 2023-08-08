#pragma once

#include "../../SDK/Interfaces/CGameEntitySystem.h"
#include "../../SDK/Interfaces/IEngineClient.h"
#include "../../SDK/Interfaces/CInputService.h"
#include "../../SDK/Interfaces/CBaseFileSystem.h"
#include "../../SDK/Interfaces/Network/CNetworkMessages.h"
#include "../../SDK/Interfaces/Network/CNetworkGameClient.h"
#include "../../SDK/Interfaces/Network/INetworkClientService.h"
#include "../../SDK/Interfaces/GC/CGCClient.h"
#include "../../SDK/Interfaces/CVarSystem.h"
#include "../../SDK/Interfaces/CFlattenedSerializers.h"
#include "../../SDK/Interfaces/CResourceSystem.h"
#include "../../SDK/Interfaces/CSoundOpSystem.h"
#include "../../SDK/Interfaces/ISteamClient.h"
#include "../../SDK/Interfaces/ISteamGameCoordinator.h"
#include "../../SDK/Interfaces/CGameUI.h"

#include "../../SDK/Panorama/CUIEngineSource2.h"

#include "../../SDK/Base/Memory.h"
#include "../../SDK/Base/Address.h"
#include "../../SDK/Base/VMT.h"
#include "../../SDK/Base/Logging.h"

#include <format>

namespace Interfaces {
	inline CCVar* CVar{};
	inline CEngineClient* Engine{}; // CEngineClient
	inline CResourceSystem* ResourceSystem{};
	inline VClass* Panorama{};
	inline CUIEngineSource2* UIEngine{};
	inline VClass* Client{}; // CSource2Client 
	inline CGameEntitySystem* EntitySystem{};
	inline ISteamClient* SteamClient{};
	inline ISteamGameCoordinator* SteamGC{};
	inline CFlattenedSerializers* FlattenedSerializers{};
	inline CSoundOpSystem* SoundOpSystem{};
	inline CGameUI* GameUI{};
	inline CGCClient* GCClient{};

	inline INetworkClientService* NetworkClientService{};
	inline CNetworkMessages* NetworkMessages{};

	inline CInputService* InputService{};
	inline VClass* Schema{};
	inline VClass* ParticleMgrSystem{};
	inline CBaseFileSystem* FileSystem{};
	// inline void* NetworkSystem;

	void FindInterfaces();
}