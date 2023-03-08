#pragma once

#include "Hooks/PrepareUnitOrders.h"
#include "Hooks/NetChannel.h"
#include "Hooks/ParticleRendering.h"
#include "Hooks/ModifierEvents.h"

#define HOOKFUNC(func) HookFunc(func, &hk##func, &o##func, #func)
#define HOOKFUNC_SIGNATURES(func) HookFunc(Signatures::##func, &hk##func, &o##func, #func)

namespace Hooks {
	static void SetUpByteHooks() {
		HOOKFUNC_SIGNATURES(PrepareUnitOrders);
		//HOOKFUNC(DispatchPacket);
		//HOOKFUNC(BAsyncSendProto);
		HOOKFUNC_SIGNATURES(CreateParticleCollection);
		HOOKFUNC_SIGNATURES(OnRemoveModifier);
	}

	static void SetUpVirtualHooks(bool log) {
		char funcAddr[60]{}, funcAddrMask[60]{};

		uintptr_t addr;
		SIGSCAN(addr, "40 53 56 57 41 56 48 83 EC ?? 45 33 F6 48 8D 71", L"networksystem.dll"); // NetChan constructor
		uintptr_t** vtable = (uintptr_t**)GetAbsoluteAddress(addr + 0x15, 3, 7); // vtable ptr at 0x15
		uintptr_t* PostReceivedNetMessage = vtable[86], *SendNetMessage = vtable[69]; // bytehooking through vtables, how's that, Elon Musk?
		HOOKFUNC(PostReceivedNetMessage);
		HOOKFUNC(SendNetMessage);
	}
}
