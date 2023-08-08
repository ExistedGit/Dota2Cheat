#include "PrepareUnitOrders.h"

bool Hooks::hkPrepareUnitOrders(CDOTAPlayerController* player, dotaunitorder_t orderType, UINT32 targetIndex, Vector* position, UINT32 abilityIndex, PlayerOrderIssuer_t orderIssuer, CBaseEntity* issuer, bool queue, bool showEffects) {
	// whether or not the execution will continue
	bool giveOrder = true;

	if (!Modules::ManaHPAbuse.IsInterruptible())
		return oPrepareUnitOrders(player, orderType, targetIndex, position, abilityIndex, orderIssuer, issuer, queue, showEffects);

	if (!issuer) { // issuer may be nullptr if it's HERO_ONLY or something
		switch (orderIssuer) {
		case DOTA_ORDER_ISSUER_HERO_ONLY:
			issuer = ctx.localHero;
			break;
		case DOTA_ORDER_ISSUER_CURRENT_UNIT_ONLY:
		case DOTA_ORDER_ISSUER_SELECTED_UNITS:
			if (ctx.localPlayer && !ctx.localPlayer->GetSelectedUnits().empty())
				issuer = Interfaces::EntitySystem->GetEntity(ctx.localPlayer->GetSelectedUnits().first());
			break;
		}
	}

	if (!issuer || issuer->GetLifeState() != 0)
		return oPrepareUnitOrders(player, orderType, targetIndex, position, abilityIndex, orderIssuer, issuer, queue, showEffects);

	switch (orderType) {
	case DOTA_UNIT_ORDER_CAST_TARGET:
	{
		Modules::CastRedirection.RedirectIfIllusionCast(targetIndex, issuer, abilityIndex, giveOrder);
		break;
	}
	case DOTA_UNIT_ORDER_CAST_POSITION: {
		Modules::PerfectBlink.AdjustIfBlink(position, abilityIndex, issuer);
		break;
	}
	case DOTA_UNIT_ORDER_CAST_NO_TARGET: {
		Modules::ManaHPAbuse.PerformAbuse((CDOTABaseNPC*)issuer, Interfaces::EntitySystem->GetEntity<CDOTAItem>(abilityIndex));
		break;
	}
	}

	if (orderType == DOTA_UNIT_ORDER_CAST_NO_TARGET ||
		orderType == DOTA_UNIT_ORDER_CAST_POSITION ||
		orderType == DOTA_UNIT_ORDER_CAST_TARGET)
		if (Modules::BadCastPrevention.IsBadCast(orderType, targetIndex, position, abilityIndex, issuer)) {
			ShowHUDError("Bad cast: no targets!", "General.CastFail_NoTarget");
			return oPrepareUnitOrders(player, orderType, targetIndex, position, abilityIndex, orderIssuer, issuer, queue, showEffects);
		}

	if (giveOrder)
		return oPrepareUnitOrders(player, orderType, targetIndex, position, abilityIndex, orderIssuer, issuer, queue, showEffects);
}
