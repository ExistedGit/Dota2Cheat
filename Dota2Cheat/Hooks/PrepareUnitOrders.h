#pragma once
#include <future>
#include "../SDK/pch.h"
#include "../CheatSDK/include.h"
#include "../Modules/Hacks/BadCastPrevention.h"
#include "../Modules/Hacks/PerfectBlink.h"
#include "../Modules/Hacks/CastRedirection.h"

// All unit orders go through this function
// This allows to adjust them to given parameters

namespace Hooks {
	inline Signatures::PrepareUnitOrdersFn oPrepareUnitOrders = nullptr;

	inline std::future<void> manaAbusePickup;

	void hkPrepareUnitOrders(CDOTAPlayerController* player, dotaunitorder_t orderType, UINT32 targetIndex, Vector* position, UINT32 abilityIndex, PlayerOrderIssuer_t orderIssuer, CBaseEntity* issuer, bool queue, bool showEffects);
}