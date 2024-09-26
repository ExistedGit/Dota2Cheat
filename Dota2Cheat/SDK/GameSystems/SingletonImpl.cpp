#include "CGlobalVars.h"
#include "CDOTAParticleManager.h"
#include "CGameParticleManagerSystem.h"
#include "CRenderGameSystem.h"
#include "CDOTA_BinaryObjectSystem.h"
#include "CDOTARichPresence.h"
#include "CDOTAGCClientSystem.h"
#include "CDOTAInventoryManager.h"
#include "CGameEventManager.h"
#include "C_DOTA_PlayerResource.h"
#include "../CheatSDK/Globals/Interfaces.h"

// Centralized file with singleton getter implementations

C_DOTA_PlayerResource** C_DOTA_PlayerResource::GetPtr() {
	static C_DOTA_PlayerResource** ptr = Address(Interfaces::Client->GetVFunc(VMI::CSource2Client::VoiceReliable)).Offset(4).GetAbsoluteAddress(3);
	return ptr;
}

CGameEventManager** CGameEventManager::GetPtr() {
	// Also in CSource2Client::Init(), right after "g_GameEventManager.Init()":
	// mov rcx, [XXXXXXXXX]
	static CGameEventManager** ptr = Address(Interfaces::Client->GetVFunc(VMI::CSource2Client::NotifyDisconnect))
		.Offset(0x3E)
		.GetAbsoluteAddress(3);

	return ptr;
}

CDOTAGCClientSystem* CDOTAGCClientSystem::Get() {
	static CDOTAGCClientSystem* inst = FindStaticGameSystem("CDOTAGCClientSystem");
	return inst;
}

CDOTARichPresence* CDOTARichPresence::Get() {
	static CDOTARichPresence* inst = FindStaticGameSystem("CDOTARichPresence");
	return inst;
}

CGlobalVars* CGlobalVars::Get() {
	return Interfaces::NetworkClientService->GetIGameClient()->GetGlobals();
}

CGameParticleManagerSystem* CGameParticleManagerSystem::Get() {
	static CGameParticleManagerSystem* inst = FindStaticGameSystem("CGameParticleManagerSystem");
	return inst;
}

CRenderGameSystem** CRenderGameSystem::GetPtr() {
	static CRenderGameSystem** ptr = FindReallocatingGameSystem("RenderGameSystem");
	return ptr;
}

CDOTAParticleManager* CDOTAParticleManager::Get() {
	return CParticleMgrSys::Get()->GetParticleManager();
}

CDOTA_BinaryObjectSystem* CDOTA_BinaryObjectSystem::Get() {
	static CDOTA_BinaryObjectSystem* inst = FindStaticGameSystem("CDOTA_BinaryObjectSystem");
	return inst;
}

CDOTAInventoryManager* CDOTAInventoryManager::Get() {
	static CDOTAInventoryManager* inst = FindStaticGameSystem("CDOTAInventoryManager");
	return inst;
}