#pragma once

// Dota 2 SDK: Universal import header

#pragma region Aliases
// Handy type aliases so my hands won't get sore

// Game systems

class C_DOTA_PlayerResource;
class CDOTAParticleManager;
class CRenderGameSystem;
class CGameEventManager;
class C_DOTA_ProjectileManager;
class CDOTA_BinaryObjectSystem;
class CGameParticleManagerSystem;
class CDOTA_MinimapObjectManager;
class CDOTAGCClientSystem;
class CDOTARichPresence;
class CDOTAInventoryManager;

using CParticleMgr = CDOTAParticleManager;
using CPlayerResource = C_DOTA_PlayerResource;
using CRenderSys = CRenderGameSystem;
using CEventMgr = CGameEventManager;
using CProjectileMgr = C_DOTA_ProjectileManager;
using CBinaryObjSys = CDOTA_BinaryObjectSystem;
using CParticleMgrSys = CGameParticleManagerSystem;
using CMinimapObjMgr = CDOTA_MinimapObjectManager;
using CGCClientSys = CDOTAGCClientSystem;
using CRichPresence = CDOTARichPresence;
using CInvMgr = CDOTAInventoryManager;

// Interfaces

class ISteamGameCoordinator;
class CGameEntitySystem;
class CSoundOpSystem;
class CBaseFileSystem;
class CUIEngineSource2;

using ISteamGC = ISteamGameCoordinator;
using CEntSys = CGameEntitySystem;
using CSoundOpSys = CSoundOpSystem;
using CFileSys = CBaseFileSystem;
using CUIEngine = CUIEngineSource2;

// Entities

class CDOTABaseNPC;
class CDOTABaseNPC_Hero;
class CBaseEntity;
class CDOTABaseAbility;
class CDOTAItem;

using CNPC = CDOTABaseNPC;
using CEnt = CBaseEntity;
using CHero = CDOTABaseNPC_Hero;
using CAbility = CDOTABaseAbility;
using CItem = CDOTAItem;

#pragma endregion

#include "Base/Definitions.h"
#include "Base/Color.h"
#include "Base/Bytehook.h"
#include "Base/VClass.h"
#include "Base/NormalClass.h"
#include "Base/Function.h"
#include "Base/Memory.h"
#include "Base/Logging.h"
#include "Base/CUtlVector.h"
#include "Base/MemAlloc.h"
#include "Base/MurmurHash2.h"
#include "Base/CUtlAbstractDelegate.h"
#include "Base/KeyValues.h"

#include "Enums.h"

#include "Entities/CHandle.h"
#include "Entities/CDOTABaseNPCHero.h"
#include "Entities/CDOTAPlayerController.h"
#include "Entities/CDOTAModifier.h"
#include "Entities/CDOTAItem.h"

#include "Interfaces/GC/CEconItem.h"
#include "Interfaces/GC/CEconWearable.h"
#include "Interfaces/GC/ItemSchema.h"
#include "Interfaces/GC/CGCClient.h"

#include "Interfaces/CBaseFileSystem.h"
#include "Interfaces/CSchemaSystem.h"
#include "Interfaces/CSource2Client.h"
#include "Interfaces/CGameEntitySystem.h"
#include "Interfaces/ISteamGameCoordinator.h"
#include "Interfaces/CGameUI.h"
#include "Interfaces/CEngineServiceMgr.h"
#include "Interfaces/CVarSystem.h"
#include "Interfaces/CFlattenedSerializers.h"
#include "Interfaces/CPanoramaUIEngine.h"
#include "Interfaces/CResourceSystem.h"
#include "Interfaces/CSoundOpSystem.h"
#include "Interfaces/ISteamClient.h"
#include "Interfaces/CInputService.h"

#include "Interfaces/Network/CNetworkGameClient.h"
#include "Interfaces/Network/CNetworkMessages.h"
#include "Interfaces/Network/INetworkClientService.h"

#include "GameSystems/C_DOTA_PlayerResource.h"
#include "GameSystems/C_DOTA_ProjectileManager.h"
#include "GameSystems/CGameEventManager.h"
#include "GameSystems/CDOTARichPresence.h"
#include "GameSystems/CDOTA_MinimapObjectManager.h"
#include "GameSystems/CDOTA_PanoramaMinimapRenderer.h"
#include "GameSystems/CGlobalVars.h"
#include "GameSystems/CDOTAParticleManager.h"
#include "GameSystems/CDOTA_BinaryObjectSystem.h"
#include "GameSystems/C_DOTAGameRules.h"
#include "GameSystems/CGameParticleManagerSystem.h"
#include "GameSystems/CRenderGameSystem.h"
#include "GameSystems/CDOTAInventoryManager.h"
#include "GameSystems/CDOTAGCClientSystem.h"

#include "Panorama/CUIEngineSource2.h"
#include "Panorama/CUIPanel.h"
#include "Panorama/CPanel2D.h"
#include "Panorama/CLocalization.h"