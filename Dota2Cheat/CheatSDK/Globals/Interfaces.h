#pragma once

#include <Interfaces/CGameEntitySystem.h>
#include <Interfaces/IEngineClient.h>
#include <Interfaces/CInputService.h>
#include <Interfaces/CBaseFileSystem.h>
#include <Interfaces/Network/CNetworkMessages.h>
#include <Interfaces/Network/CNetworkGameClient.h>
#include <Interfaces/Network/INetworkClientService.h>
#include <Interfaces/GC/CGCClient.h>
#include <Interfaces/CVarSystem.h>
#include <Interfaces/CFlattenedSerializers.h>
#include <Interfaces/CResourceSystem.h>
#include <Interfaces/CSoundOpSystem.h>
#include <Interfaces/ISteamClient.h>
#include <Interfaces/ISteamGameCoordinator.h>
#include <Interfaces/CGameUI.h>
#include <Interfaces/CSource2Client.h>
#include <Interfaces/CPanoramaUIEngine.h>
#include <Interfaces/IEngineClient.h>
#include <Panorama/CUIEngineSource2.h>

using ISteamGC = ISteamGameCoordinator;
using CEntSys = CGameEntitySystem;
using CSoundOpSys = CSoundOpSystem;
using CFileSys = CBaseFileSystem;
using CUIEngine = CUIEngineSource2;