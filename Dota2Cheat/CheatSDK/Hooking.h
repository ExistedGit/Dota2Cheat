#pragma once
#include "../SDK/pch.h"
#include "EventListeners.h"

#include "../Hooks/PrepareUnitOrders.h"
#include "../Hooks/AcceptEvents.h"
#include "../Hooks/RunFrame.h"
#include "../Hooks/NetChannel.h"
#include "../Hooks/GameCoordinator.h"
#include "../Hooks/ParticleRendering.h"
#include "../Hooks/SteamGC.h"
#include "../Hooks/ModifierEvents.h"
#include "../Hooks/Present.h"

#include "../Hooks/Misc.h"

namespace Hooks {
	inline EntityEventListener* EntEventListener{};

	void InstallHooks();

	// Removes any custom, non-MinHook hooks
	inline void RemoveHooks() {
		Interfaces::EntitySystem->GetListeners().remove_by_value(EntEventListener);
		CMemAlloc::Instance()->Free(EntEventListener);
		Interfaces::UIEngine->GetListeners().remove_by_value(hkRunFrame);
	}
}
