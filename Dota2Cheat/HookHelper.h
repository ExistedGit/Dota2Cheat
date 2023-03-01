#pragma once

#include "Hooks/PrepareUnitOrders.h"
#include "Hooks/NetChannel.h"
#include "Hooks/ParticleRendering.h"
#include "Hooks/ModifierEvents.h"

#define HOOKFUNC(func) HookFunc(##func, &hk##func, &o##func, #func)
#define HOOKFUNC_SIGNATURES(func) HookFunc(Signatures::##func, &hk##func, &o##func, #func)

namespace Hooks {
	inline void SetUpByteHooks() {
		//HOOKFUNC(PopupAcceptMatch);
		HOOKFUNC_SIGNATURES(PrepareUnitOrders);
		//HOOKFUNC(DispatchPacket);
		//HOOKFUNC(BAsyncSendProto);
		HOOKFUNC_SIGNATURES(CreateParticleCollection);
		HOOKFUNC_SIGNATURES(OnRemoveModifier);
	}

	inline void SetUpVirtualHooks(bool log) {
		//VMTs::NetworkSystem = std::unique_ptr<VMT>(new VMT(Interfaces::NetworkSystem));
		//VMTs::NetworkSystem->HookVM(CreateNetChannel, 26);
		//VMTs::NetworkSystem->ApplyVMT();
		
		
		char funcAddr[60]{}, funcAddrMask[60]{};

		uintptr_t addr;
		SIGSCAN(addr, "40 53 56 57 41 56 48 83 EC ?? 45 33 F6 48 8D 71", L"networksystem.dll"); // NetChan constructor
		uintptr_t** vtable = (uintptr_t**)GetAbsoluteAddress(addr + 0x15, 3, 7); // vtable ptr at 0x15
		auto PostReceivedNetMessage = vtable[86]; // bytehooking through vtables, how's that, Elon Musk?
		HOOKFUNC(PostReceivedNetMessage);

	}
}
