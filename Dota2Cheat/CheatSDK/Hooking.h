#pragma once
#include "../pch.h"

#include "../Hooks/PrepareUnitOrders.h"
#include "../Hooks/AcceptEvents.h"
#include "../Hooks/FrameStageNotify.h"
#include "../Hooks/NetChannel.h"
#include "../Hooks/GameCoordinator.h"
#include "../Hooks/ParticleRendering.h"
#include "../Hooks/SteamGC.h"
#include "../Hooks/ModifierEvents.h"
#include "../Hooks/Present.h"

#include "../Hooks/Misc.h"

namespace Hooks {
	void InstallHooks();

	// Removes any custom, non-MinHook hooks
	inline void RemoveHooks() {
		CEntSys::Get()->GetListeners().remove_by_value(&EntityList);
	}
}
