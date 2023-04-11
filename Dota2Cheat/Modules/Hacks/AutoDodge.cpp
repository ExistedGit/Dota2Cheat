#include "../../SDK/pch.h"
#include "AutoDodge.h"

void Hacks::AutoDodge::FrameBasedLogic() {
	if (!Config::AutoDodge::Enabled
		|| !GameSystems::ProjectileManager)
		return;

	if (!ctx.ImportantItems["manta"] && !ctx.ImportantItems["bottle"])
		return;

	for (auto& proj : GameSystems::ProjectileManager->GetTrackingProjectiles()) {
		if (!proj || proj->IsAttack())
			continue;

		auto target = proj->GetTarget();
		auto source = proj->GetSource();

		if (target != ctx.assignedHero ||
			(source && source->IsSameTeam(ctx.assignedHero)))
			continue;

		//if (counterspell && counterspell->GetCooldown() == 0)
		//	useTime = 1.2f;


		if (!IsWithinRadius(proj->PredictPos(0.05f), target->GetPos(), 24.0f /*target->GetHullRadius()*/))
			continue;


		auto usedItem = ctx.ImportantItems["bottle"] ? ctx.ImportantItems["bottle"] : ctx.ImportantItems["manta"];
		//if (counterspell && counterspell->GetCooldown() == 0)
		//	usedItem = counterspell;
		//else
		if (!usedItem ||
			(
				usedItem == ctx.ImportantItems["bottle"] && // if we can use bottle
				usedItem->Member<RuneType>(Netvars::C_DOTA_Item_EmptyBottle::m_iStoredRuneType) != DOTA_RUNE_ILLUSION // and it has no illusion rune
				&& !((usedItem = ctx.ImportantItems["manta"]) && // and if we don't have manta
					usedItem->GetCooldown() == 0) // or it's on cooldown
				)
			)
			continue;

		ctx.localPlayer->PrepareOrder(
			DOTA_UNIT_ORDER_CAST_NO_TARGET,
			0,
			&Vector::Zero,
			usedItem->GetIndex(),
			DOTA_ORDER_ISSUER_PASSED_UNIT_ONLY,
			ctx.assignedHero);
		break;
	}
}
