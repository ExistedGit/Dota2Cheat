#include "Hooking.h"

#define HOOKFUNC_SIGNATURES(func) HookFunc(Signatures::##func, &hk##func, &o##func, #func)

void Hooks::InstallHooks() {
	HOOKFUNC_SIGNATURES(PrepareUnitOrders);
	HOOKFUNC_SIGNATURES(CDOTA_DB_Popup_AcceptMatch);

#if defined(_DEBUG) && !defined(_TESTING)
	{
		auto SendMsg = VMT(Interfaces::SteamGC).GetVM(0);
		HookFunc(SendMsg, &Hooks::hkSendMessage, &Hooks::oSendMessage, "ISteamGameCoordinator::SendMessage");
	}
	HOOKFUNC_SIGNATURES(SaveSerializedSOCache);
#endif // _DEBUG
	{
	{
		auto RetrieveMessage = VMT(Interfaces::SteamGC).GetVM(2);
		HookFunc(RetrieveMessage, &Hooks::hkRetrieveMessage, &Hooks::oRetrieveMessage, "ISteamGameCoordinator::RetrieveMessage");
	}
		//uintptr_t** vtable = Memory::Scan("48 8D 05 ? ? ? ? 48 8B D9 48 89 01 48 8B 49 50", "client.dll").GetAbsoluteAddress(3);
		//uintptr_t* SetAbsOrigin = vtable[21];
		//HOOKFUNC(SetAbsOrigin);
	}

	{
		// NetChan constructor
		// vtable ptr at 0x15
		uintptr_t** vtable = Memory::Scan("40 53 56 57 41 56 48 83 EC ? 45 33 F6 48 8D 71", "networksystem.dll")
			.Offset(0x15)
			.GetAbsoluteAddress(3, 7);
		uintptr_t* PostReceivedNetMessage = vtable[86], * SendNetMessage = vtable[69]; // bytehooking through vtables, how's that, Elon Musk?
		HOOKFUNC(PostReceivedNetMessage);
		HOOKFUNC(SendNetMessage);
	}
	{
		// CDOTA_Buff destructor
		// vtable ptr at 0xd
		auto OnRemoveModifier = Memory::Scan("4C 8B DC 56 41 57", "client.dll");
		uintptr_t** vtable = OnRemoveModifier.Offset(0xd).GetAbsoluteAddress(3);
		uintptr_t* OnAddModifier = vtable[VTableIndexes::CDOTA_Buff::OnAddModifier];
		HOOKFUNC(OnAddModifier);
		HOOKFUNC(OnRemoveModifier);
	}
	{
		// xref: "CParticleCollection::~CParticleCollection [%p]\n"
		auto particleDestructor = Memory::Scan("E8 ? ? ? ? 40 F6 C7 01 74 34", "particles.dll")
			.GetAbsoluteAddress(1);
		uintptr_t** vtable = particleDestructor
			.Offset(0x19)
			.GetAbsoluteAddress(3);
		auto SetRenderingEnabled = vtable[VTableIndexes::CParticleCollection::SetRenderingEnabled];
		HOOKFUNC(SetRenderingEnabled);
	}
	//{
	//	void* RunScript = Interfaces::UIEngine->GetVFunc(88).ptr;
	//	HOOKFUNC(RunScript);
	//}
	{
		// Hooking HUD flip's callback to avoid sigging IsHUDFlipped
		// (and lowering performance, according to Wolf49406...)
		auto cvar = Interfaces::CVar->CVars["dota_hud_flip"];
		auto& callback = Interfaces::CVar->GetCallback(cvar.m_pVar->m_iCallbackIndex);
		oOnHUDFlipped = callback;
		HUDFlipCallback = (void**)&callback;
		callback = hkOnHUDFlipped;
	}
	{
		EntEventListener = CMemAlloc::Instance()->AllocInit<EntityEventListener>();
		Interfaces::EntitySystem->GetListeners().push_back(EntEventListener);

		Interfaces::UIEngine->GetListeners().push_back(Hooks::hkRunFrame);
	}
}

