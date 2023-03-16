#pragma once

#include "Hooks/PrepareUnitOrders.h"
#include "Hooks/RunFrame.h"
#include "Hooks/NetChannel.h"
#include "Hooks/EntitySystemEvents.h"
#include "Hooks/ParticleRendering.h"
#include "Hooks/ModifierEvents.h"

#define HOOKFUNC(func) HookFunc(func, &hk##func, &o##func, #func)
#define HOOKFUNC_SIGNATURES(func) HookFunc(Signatures::##func, &hk##func, &o##func, #func)

namespace Hooks {
	inline std::set<void*> hooks;

	void SetUpByteHooks();

	void SetUpVirtualHooks(bool log);
	void DisableHooks();
	void EnableHooks();
}
