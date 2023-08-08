#pragma once
#include <future>
#include "../pch.h"
#include "../CheatSDK/include.h"
#include "../Modules/Hacks/BadCastPrevention.h"
#include "../Modules/Hacks/PerfectBlink.h"
#include "../Modules/Hacks/CastRedirection.h"
#include "../Modules/Hacks/ManaHPAbuse.h"

// All unit orders go through this function
// This allows to adjust them to given parameters

namespace Hooks {
	inline Signatures::PrepareUnitOrdersFn oPrepareUnitOrders = nullptr;
	bool hkPrepareUnitOrders(CDOTAPlayerController* player, dotaunitorder_t orderType, UINT32 targetIndex, Vector* position, UINT32 abilityIndex, PlayerOrderIssuer_t orderIssuer, CBaseEntity* issuer, bool queue, bool showEffects);
}