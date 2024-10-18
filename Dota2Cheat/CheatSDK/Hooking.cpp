#include "../pch.h"
#include "Hooking.h"

#include "Aliases.h"

#include <Base/VMT.h>

#include "../Hooks/PrepareUnitOrders.h"
#include "../Hooks/AcceptEvents.h"
#include "../Hooks/FrameStageNotify.h"
#include "../Hooks/Network.h"
#include "../Hooks/GameCoordinator.h"
#include "../Hooks/ParticleRendering.h"
#include "../Hooks/SteamGC.h"
#include "../Hooks/ModifierEvents.h"
#include "../Hooks/Present.h"

#include "../Hooks/Misc.h"

void Hooks::InstallHooks() {
	hooks::Hook(Signatures::PrepareUnitOrders, &Hooks::hkPrepareUnitOrders, &Hooks::oPrepareUnitOrders, "CDOTAPlayerController::PrepareUnitOrders");

#ifdef _DEBUG
	auto BShowRestrictedAddonPopup = SignatureDB::FindSignature("BShowRestrictedAddonPopup");
	HOOKFUNC(BShowRestrictedAddonPopup);
#endif

#if defined(_DEBUG) && !defined(_TESTING)
	auto SendMsg = VMT(ISteamGC::Get())[0];
	auto RetrieveMessage = VMT(ISteamGC::Get())[2];
	hooks::Hook(SendMsg, &Hooks::hkSendMessage, &Hooks::oSendMessage, "ISteamGameCoordinator::SendMessage");
	hooks::Hook(RetrieveMessage, &Hooks::hkRetrieveMessage, &Hooks::oRetrieveMessage, "ISteamGameCoordinator::RetrieveMessage");
#endif // _DEBUG

	{
		// CDOTA_Buff destructor
		// vtable ptr at 0xd
		auto OnRemoveModifier = SignatureDB::FindSignature("CDOTA_Buff::~CDOTA_Buff");
		uintptr_t** vtable = OnRemoveModifier.Offset(0xD).GetAbsoluteAddress(3);
		uintptr_t* OnAddModifier = vtable[VMI::CDOTA_Buff::OnAddModifier];
		HOOKFUNC(OnAddModifier);
		HOOKFUNC(OnRemoveModifier);
		CDOTAParticleManager::DestroyParticleFunc = OnRemoveModifier.Offset(0x72).GetAbsoluteAddress(1);
	}
	{
		// xref: "CParticleCollection::~CParticleCollection [%p]\n"
		uintptr_t** vtable = SignatureDB::FindSignature("CParticleCollection::CParticleCollection");
		auto SetRenderingEnabled = vtable[VMI::CParticleCollection::SetRenderingEnabled];
		HOOKFUNC(SetRenderingEnabled);
	}
	auto Present = SignatureDB::FindSignature("IDXGISwapChain::Present");
	HOOKFUNC(Present);

#ifndef _TESTING
	void* RunScript = CUIEngine::Get()->GetVFunc(VMI::CUIEngineSource2::RunScript);
	HOOKFUNC(RunScript);
	LogF("RunScript: {}", RunScript);
#endif

	//{
	//	auto client = INetworkClientService::Get()->GetIGameClient();
	//	auto nc = client->GetNetChannel();
	//	auto buf = ((VClass*)client)->MemberInline<void>(40);
	//	
	//	auto pbs = CNetworkMessages::Get()->FindNetworkMessageByID(4);
	//	CUtlAbstractDelegate d(client, meme);
	//	nc->StartRegisteringMessageHandlers();
	//	nc->RegisterAbstractMessageHandler(buf, &d, 1, pbs, 0);
	//	nc->FinishRegisteringMessageHandlers();
	//}
}

void Hooks::InstallAuxiliaryHooks() {
	CEntSys::Get()->GetListeners().push_back(&EntityList);

	CEngineServiceMgr::Get()
		->GetEventDispatcher()
		->RegisterEventListener_Abstract(
			CUtlAbstractDelegate(&Hooks::frameListener, &Hooks::FrameEventListener::OnFrameBoundary),
			"EventFrameBoundary_t",
			"D2C::OnFrameBoundary"
		);
}

// Removes any custom, non-MinHook hooks
void Hooks::RemoveAuxiliaryHooks() {
	CEngineServiceMgr::Get()
		->GetEventDispatcher()
		->UnregisterEventListener_Abstract(
			CUtlAbstractDelegate(&Hooks::frameListener, &Hooks::FrameEventListener::OnFrameBoundary),
			"EventFrameBoundary_t"
		);

	CEntSys::Get()->GetListeners().remove_by_value(&EntityList);
	INetworkClientService::Get()->GetIGameClient()->GetNetChannel()->UninstallMessageFilter(&d2cNetFilter);
}

