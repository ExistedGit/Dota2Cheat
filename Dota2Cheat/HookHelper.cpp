#include "HookHelper.h"
#include "SDK/Base/VMT.h"

#define HOOKFUNC_INGAME(func) HOOKFUNC(func); hooks.insert(func);
#define HOOKFUNC_SIGNATURES_INGAME(func) HOOKFUNC_SIGNATURES(func); hooks.insert(Signatures::func);

void Hooks::SetUpByteHooks() {
	HOOKFUNC_SIGNATURES(PrepareUnitOrders);
	//HOOKFUNC(DispatchPacket);
	//HOOKFUNC(BAsyncSendProto);
	HOOKFUNC_SIGNATURES_INGAME(CreateParticleCollection);
	HOOKFUNC_SIGNATURES(OnRemoveModifier);
}


void Hooks::SetUpVirtualHooks(bool log) {

	SigScanContext ssctx{ ctx.CurProcHandle, ctx.CurProcId };
	// NetChan constructor
	// vtable ptr at 0x15
	uintptr_t** vtable = ssctx.Scan("40 53 56 57 41 56 48 83 EC ?? 45 33 F6 48 8D 71", L"networksystem.dll").Offset(0x15).GetAbsoluteAddress(3, 7);
	uintptr_t* PostReceivedNetMessage = vtable[86], * SendNetMessage = vtable[69]; // bytehooking through vtables, how's that, Elon Musk?
	HOOKFUNC_INGAME(PostReceivedNetMessage);
	HOOKFUNC_INGAME(SendNetMessage);

	{
		auto vmt = VMT(Interfaces::EntitySystem);
		HookFunc(vmt.GetVM<EntSystemEvent>(14), &OnAddEntity, &oOnAddEntity, "OnAddEntity");
		HookFunc(vmt.GetVM<EntSystemEvent>(15), &OnRemoveEntity, &oOnRemoveEntity, "OnRemoveEntity");
	}
	{
		auto vmt = VMT(Interfaces::UIEngine);
		HookFunc(vmt.GetVM<RunFrameFn>(6), &hkRunFrame, &oRunFrame, "RunFrame");
	}
}

void Hooks::DisableHooks() {
	for(auto hook : hooks)
		MH_DisableHook(hook);
}

void Hooks::EnableHooks() {
	for (auto hook : hooks)
		MH_EnableHook(hook);
}
