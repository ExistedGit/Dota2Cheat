#include "AutoDodge.h"

void Hacks::AutoDodge::FrameBasedLogic() {
	if (!Config::AutoDodge::Enabled
		|| !GameSystems::ProjectileManager)
		return;

	if (!HeroData[ctx.localHero].Items["manta"] && !HeroData[ctx.localHero].Items["bottle"])
		return;

	for (auto proj : GameSystems::ProjectileManager->m_pTrackingProjectiles) {
		if (!proj || proj->IsAttack())
			continue;

		auto target = proj->GetTarget();
		auto source = proj->GetSource();

		if (target != ctx.localHero ||
			(source && source->IsSameTeam(ctx.localHero)))
			continue;

		//if (counterspell && counterspell->GetCooldown() == 0)
		//	useTime = 1.2f;

		{
			auto dist = proj->GetPos().As<Vector2D>().DistTo(target->GetPos().As<Vector2D>()) - target->GetHullRadius();
			auto timeToHit = dist / proj->GetMoveSpeed();
			if (timeToHit > 0.05f)
				continue;
		}

		auto usedItem = HeroData[ctx.localHero].Items["bottle"] ? HeroData[ctx.localHero].Items["bottle"] : HeroData[ctx.localHero].Items["manta"];
		//if (counterspell && counterspell->GetCooldown() == 0)
		//	usedItem = counterspell;
		//else
		if (!usedItem ||
			(
				usedItem == HeroData[ctx.localHero].Items["bottle"] && // if we can use bottle
				usedItem->Member<RuneType>(Netvars::C_DOTA_Item_EmptyBottle::m_iStoredRuneType) != DOTA_RUNE_ILLUSION // and it has no illusion rune
				&& !((usedItem = HeroData[ctx.localHero].Items["manta"]) && // and if we don't have manta
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
			ctx.localHero);
		break;
	}
}
