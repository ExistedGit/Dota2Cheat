#pragma once

#include "Hooks/PrepareUnitOrders.h"
#include "Hooks/NetChannel.h"

namespace Hooks {
	class HookHelper {
	public:
		static void Hook(void* func, void* detour, void* original, const std::string& name) {
			if (MH_CreateHook(func, detour,
				(LPVOID*)original) != MH_OK ||
				MH_EnableHook(func) != MH_OK)
				std::cout << "Could not hook" << name << "()!\n";
		};

		static void SetUpByteHooks() {
			Hook(Signatures::PrepareUnitOrders, &hkPrepareUnitOrders, &PrepareUnitOrdersOriginal, "PrepareUnitOrders");
			Hook(Signatures::DispatchPacket, &hkDispatchPacket, &DispatchPacketOriginal, "DispatchPacket");
			Hook(Signatures::BAsyncSendProto, &hkBAsyncSendProto, &BAsyncSendProtoOriginal, "BAsyncSendProto");
		}

		static void SetUpVirtualHooks() {
			VMTs::NetworkSystem = std::unique_ptr<VMT>(new VMT(Interfaces::NetworkSystem));
			VMTs::NetworkSystem->HookVM(CreateNetChannel, 26);
			VMTs::NetworkSystem->ApplyVMT();
		}
	};
}
