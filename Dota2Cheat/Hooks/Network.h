#pragma once
#include "../SDK/Interfaces/Network/INetChannel.h"

#include <netmessages.pb.h>

namespace Hooks {
	inline struct D2CNetFilter : public INetworkMessageProcessingPreFilter {
		bool Filter(VClass* msg, INetChannel* netchan) override;
	} d2cNetFilter;
}