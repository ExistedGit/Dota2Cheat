#pragma once

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

class CDOTABaseNPC;
class CDOTABaseNPC_Hero;
class CBaseEntity;

using CNPC = CDOTABaseNPC;
using CEnt = CBaseEntity;
using CHero = CDOTABaseNPC_Hero;