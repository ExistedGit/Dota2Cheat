#include "Hooking.h"
#include <Base/VMT.h>

bool meme(void* rcx, const CNETMsg_Tick* msg) {
	return 1;
}

void Hooks::InstallHooks() {
	hooks::Hook(Signatures::PrepareUnitOrders, &Hooks::hkPrepareUnitOrders, &Hooks::oPrepareUnitOrders, "CDOTAPlayerController::PrepareUnitOrders");

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

	void* FrameStageNotify = CSource2Client::Get()->GetVFunc(VMI::CSource2Client::FrameStageNotify);
	HOOKFUNC(FrameStageNotify);

#ifndef _TESTING
	void* RunScript = CUIEngine::Get()->GetVFunc(VMI::CUIEngineSource2::RunScript);
	HOOKFUNC(RunScript);
#endif

	auto CreateNetChan = SignatureDB::FindSignature("CNetworkSystem::CreateNetChan");
	HOOKFUNC(CreateNetChan);

	//{
	//	auto PacketEntitiesFilter__Filter = SignatureDB::FindSignature("PacketEntitiesFilter::Filter");
	//	HOOKFUNC(PacketEntitiesFilter__Filter);
	//}
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

