#pragma once
#include "../pch.h"
#include "../SDK/Interfaces/Network/INetChannel.h"

#include <netmessages.pb.h>

namespace Hooks {

	struct D2CNetFilter : public INetworkMessageProcessingPreFilter {
		bool Filter(VClass* msg, INetChannel* netchan) override;
	};

	inline D2CNetFilter d2cNetFilter;

	inline void* oCreateNetChan{};
	inline void* oPacketEntitiesFilter__Filter{};

	inline bool hkPacketEntitiesFilter__Filter(VClass* msg, INetChannel* netchan) {
		d2cNetFilter.Filter(msg, netchan);
		return ORIGCALL(PacketEntitiesFilter__Filter)(msg, netchan);
	}
	inline INetChannel* hkCreateNetChan(void* netsys, int a2, void* a3, unsigned a4, const char* a5, int a6, int a7) {
		auto res = ORIGCALL(CreateNetChan)(netsys, a2, a3, a4, a5, a6, a7);
		res->InstallMessageFilter(&d2cNetFilter);
		return res;
	}
}