#pragma once
#include "../../pch.h"
#include "../../CheatSDK/include.h"
#include "../Hacks/PerfectBlink.h"
#include "../Hacks/CastRedirection.h"
#include "../Hacks/BadCastPrevention.h"

namespace Modules {
	inline class M_OrderRouter {
	public:
		
		bool RouteOrder(NetMessageHandle_t* messageHandle, google::protobuf::Message* msg);
	} OrderRouter{};
}