#pragma once
#include "../SDK/pch.h"
#include "EventListeners.h"

#include "../Hooks/PrepareUnitOrders.h"
#include "../Hooks/AcceptEvents.h"
#include "../Hooks/RunFrame.h"
#include "../Hooks/NetChannel.h"
#include "../Hooks/GameCoordinator.h"
#include "../Hooks/EntityEvents.h"
#include "../Hooks/ParticleRendering.h"
#include "../Hooks/SteamGC.h"
#include "../Hooks/ModifierEvents.h"

#include "../Hooks/Misc.h"

namespace Hooks {
	inline void** HUDFlipCallback{};
	inline EntityEventListener* EntEventListener{};
	void InstallHooks();

	// Removes any custom, non-MinHook hooks
	inline void RemoveHooks() {
		if (HUDFlipCallback)
			*HUDFlipCallback = oOnHUDFlipped;

		Interfaces::EntitySystem->GetListeners().remove_by_value(EntEventListener);
		CMemAlloc::Instance()->Free(EntEventListener);
		Interfaces::UIEngine->GetListeners().remove_by_value(hkRunFrame);
	}
}
