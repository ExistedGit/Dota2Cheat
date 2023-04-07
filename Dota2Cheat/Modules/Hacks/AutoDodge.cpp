#include "../../SDK/pch.h"
#include "AutoDodge.h"

void Hacks::AutoDodge::FrameBasedLogic() {
	if (!Config::AutoDodge::Enabled
		|| !GameSystems::ProjectileManager)
		return;

	for (auto& proj : GameSystems::ProjectileManager->GetTrackingProjectiles()) {
		if (!proj || proj->IsAttack() ||
			(!ctx.importantItems.manta && !ctx.importantItems.bottle))
			continue;

		auto target = proj->GetTarget();
		auto source = proj->GetSource();

		if (target != ctx.assignedHero ||
			(source && source->GetTeam() == ctx.assignedHero->GetTeam()))
			continue;

		//if (counterspell && counterspell->GetCooldown() == 0)
		//	useTime = 1.2f;


		if (!IsWithinRadius(proj->PredictPos(0.05f), target->GetPos(), 24.0f /*target->GetHullRadius()*/))
			continue;


		auto usedItem = ctx.importantItems.bottle ? ctx.importantItems.bottle : ctx.importantItems.manta;
		//if (counterspell && counterspell->GetCooldown() == 0)
		//	usedItem = counterspell;
		//else
		if (!usedItem ||
			(
				usedItem == ctx.importantItems.bottle && // if we can use bottle
				usedItem->Member<DotaRunes>(Netvars::C_DOTA_Item_EmptyBottle::m_iStoredRuneType) != DotaRunes::ILLUSION // and it has no illusion rune
				&& !((usedItem = ctx.importantItems.manta) && // and if we don't have manta
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
