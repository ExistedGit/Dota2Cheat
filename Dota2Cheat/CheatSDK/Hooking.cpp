#include "Hooking.h"

#define HOOKFUNC_SIGNATURES(func) HookFunc(Signatures::##func, &hk##func, &o##func, #func)

void Hooks::InstallHooks() {
	HOOKFUNC_SIGNATURES(PrepareUnitOrders);

#if defined(_DEBUG) && !defined(_TESTING)
	{
		auto SendMsg = VMT(Interfaces::SteamGC)[0];
		auto RetrieveMessage = VMT(Interfaces::SteamGC)[2];
		HookFunc(SendMsg, &Hooks::hkSendMessage, &Hooks::oSendMessage, "ISteamGameCoordinator::SendMessage");
		HookFunc(RetrieveMessage, &Hooks::hkRetrieveMessage, &Hooks::oRetrieveMessage, "ISteamGameCoordinator::RetrieveMessage");
	}
	HOOKFUNC_SIGNATURES(SaveSerializedSOCache);
#endif // _DEBUG

#ifdef _TESTING
	{
	}
#endif

	{
		// NetChan constructor
		// vtable ptr at 0x15
		uintptr_t** vtable = SignatureDB::FindSignature("CNetChan::vftable");
		uintptr_t* PostReceivedNetMessage = vtable[71], * SendNetMessage = vtable[34]; // bytehooking through vtables, how's that, Elon Musk?
		HOOKFUNC(PostReceivedNetMessage);
		// HOOKFUNC(SendNetMessage);
	}
	{
		// CDOTA_Buff destructor
		// vtable ptr at 0xd
		auto OnRemoveModifier = SignatureDB::FindSignature("CDOTA_Buff::~CDOTA_Buff");
		uintptr_t** vtable = OnRemoveModifier.Offset(0xD).GetAbsoluteAddress(3);
		uintptr_t* OnAddModifier = vtable[VTableIndexes::CDOTA_Buff::OnAddModifier];
		HOOKFUNC(OnAddModifier);
		HOOKFUNC(OnRemoveModifier);
		CDOTAParticleManager::DestroyParticleFunc = OnRemoveModifier.Offset(0x72).GetAbsoluteAddress(1);
	}
	{
		// xref: "CParticleCollection::~CParticleCollection [%p]\n"
		uintptr_t** vtable = SignatureDB::FindSignature("CParticleCollection::CParticleCollection");
		auto SetRenderingEnabled = vtable[VTableIndexes::CParticleCollection::SetRenderingEnabled];
		HOOKFUNC(SetRenderingEnabled);
	}
	{


		void* FrameStageNotify = Interfaces::Client->GetVFunc(31);
		HOOKFUNC(FrameStageNotify);

#ifndef _TESTING
		void* RunScript = Interfaces::UIEngine->GetVFunc(VTableIndexes::CUIEngineSource2::RunScript);
		HOOKFUNC(RunScript);
#endif

	}
	
	Interfaces::EntitySystem->GetListeners().push_back(&EntityList);

	HookDX11Old();
}

