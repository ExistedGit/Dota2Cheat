#include "GameSystems.h"
#include "Interfaces.h"

// Centralized file with singleton getter implementations

C_DOTA_PlayerResource** C_DOTA_PlayerResource::GetPtr() {
	// Found by xrefing this global in dylibs
	// look below the vfunc with xrefs "ehandle", "%d (s/n %d)", "[-1] -> empty", "m_flPoseParameter", "%s(%s)", "[%d %d] -> %s", "CStrongHandle", "CWeakHandle"
	static C_DOTA_PlayerResource** ptr = 
		Address(
			CSource2Client::Get()->GetVFunc(VMI::CSource2Client::VoiceReliable)
		)
		.Offset(4)
		.GetAbsoluteAddress(3);

	return ptr;
}

CGameEventManager** CGameEventManager::GetPtr() {
	// Also in CSource2Client::Init(), right after "g_GameEventManager.Init()":
	// mov rcx, [XXXXXXXXX]
	static CGameEventManager** ptr = 
		Address(
			CSource2Client::Get()->GetVFunc(VMI::CSource2Client::NotifyDisconnect)
		)
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
	return INetworkClientService::Get()->GetIGameClient()->GetGlobals();
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