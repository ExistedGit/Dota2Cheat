#include "Hooking.h"

#define HOOKFUNC_SIGNATURES(func) HookFunc(Signatures::##func, &hk##func, &o##func, #func)

void Hooks::InstallHooks() {
	HOOKFUNC_SIGNATURES(PrepareUnitOrders);
#if defined(_DEBUG) && !defined(_TESTING)
	{
		auto SendMsg = VMT(Interfaces::SteamGC).GetVM(0);
		HookFunc(SendMsg, &Hooks::hkSendMessage, &Hooks::oSendMessage, "ISteamGameCoordinator::SendMessage");
	}
	HOOKFUNC_SIGNATURES(SaveSerializedSOCache);
#endif // _DEBUG

#ifdef _TESTING
	{
		auto RetrieveMessage = VMT(Interfaces::SteamGC).GetVM(2);
		HookFunc(RetrieveMessage, &Hooks::hkRetrieveMessage, &Hooks::oRetrieveMessage, "ISteamGameCoordinator::RetrieveMessage");
	}
#endif
	{
		// NetChan constructor
		// vtable ptr at 0x15
		uintptr_t** vtable = SignatureDB::FindSignature("CNetChan::vftable");
		uintptr_t* PostReceivedNetMessage = vtable[86], * SendNetMessage = vtable[69]; // bytehooking through vtables, how's that, Elon Musk?
		HOOKFUNC(PostReceivedNetMessage);
		HOOKFUNC(SendNetMessage);
	}
	{
		// CDOTA_Buff destructor
		// vtable ptr at 0xd
		auto OnRemoveModifier = SignatureDB::FindSignature("CDOTA_Buff::~CDOTA_Buff");
		uintptr_t** vtable = OnRemoveModifier.Offset(0xd).GetAbsoluteAddress(3);
		uintptr_t* OnAddModifier = vtable[VTableIndexes::CDOTA_Buff::OnAddModifier];
		HOOKFUNC(OnAddModifier);
		HOOKFUNC(OnRemoveModifier);
	}
	{
		// xref: "CParticleCollection::~CParticleCollection [%p]\n"
		uintptr_t** vtable = SignatureDB::FindSignature("CParticleCollection::~CParticleCollection");
		auto SetRenderingEnabled = vtable[VTableIndexes::CParticleCollection::SetRenderingEnabled];
		HOOKFUNC(SetRenderingEnabled);
	}
	{
		void* RunScript = Interfaces::UIEngine->GetVFunc(88).ptr;
		HOOKFUNC(RunScript);
	}
	{
		EntEventListener = CMemAlloc::Instance()->AllocInit<EntityEventListener>();
		Interfaces::EntitySystem->GetListeners().push_back(EntEventListener);

		Interfaces::UIEngine->GetListeners().push_back(Hooks::hkRunFrame);
	}
	Hooks::HookDirectX();
}

