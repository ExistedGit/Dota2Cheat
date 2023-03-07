#pragma once
#include <future>
#include "../SDK/include.h"
#include "../Modules/Hacks/BadCastPrevention.h"
#include "../Modules/Hacks/PerfectBlink.h"
#include "../Modules/Hacks/CastRedirection.h"

namespace Hooks {
	inline Signatures::PrepareUnitOrdersFn oPrepareUnitOrders = nullptr;

	inline std::future<void> manaAbusePickup;

	void ChangeItemStatTo(CDOTAItem* item, ItemStat_t stat, CDOTAPlayerController* player, CBaseEntity* issuer);

	void hkPrepareUnitOrders(CDOTAPlayerController* player, dotaunitorder_t orderType, UINT32 targetIndex, Vector* position, UINT32 abilityIndex, PlayerOrderIssuer_t orderIssuer, CBaseEntity* issuer, bool queue, bool showEffects);
}