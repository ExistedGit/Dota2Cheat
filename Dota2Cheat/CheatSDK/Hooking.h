#pragma once
#include "../SDK/pch.h"

#include "../Hooks/PrepareUnitOrders.h"
#include "../Hooks/AcceptEvents.h"
#include "../Hooks/RunFrame.h"
#include "../Hooks/NetChannel.h"
#include "../Hooks/EntityEvents.h"
#include "../Hooks/ParticleRendering.h"
#include "../Hooks/ModifierEvents.h"
#include "../Hooks/GameEvents.h"

#include "../Hooks/Misc.h"

#define HOOKFUNC_SIGNATURES(func) HookFunc(Signatures::##func, &hk##func, &o##func, #func)

namespace Hooks {
	void SetUpByteHooks();

	void SetUpVirtualHooks(bool log);
}
