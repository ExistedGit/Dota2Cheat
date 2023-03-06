#pragma once
#include "../../SDK/include.h"

namespace Config {
#ifdef _DEBUG
	inline bool AutoDodge = true;
#else
	inline bool AutoDodge = false;
#endif // _DEBUG

}

namespace Hacks {
	class AutoDodge {
	public:
		void FrameBasedLogic() {
			if (!Config::AutoDodge)
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

				if (!IsWithinRadius(proj->PredictPos(0.05f), target->GetPos(), target->GetHullRadius()))
					continue;

				auto usedItem = ctx.importantItems.bottle ? ctx.importantItems.bottle : ctx.importantItems.manta;
				//if (counterspell && counterspell->GetCooldown() == 0)
				//	usedItem = counterspell;
				//else
				if (!usedItem ||
					(
						usedItem == ctx.importantItems.bottle && // if we can use bottle
						usedItem->Member<DotaRunes>(Netvars::C_DOTA_Item_EmptyBottle::m_iStoredRuneType) != DotaRunes::ILLUSION // and it has no illusion rune
						&& !((usedItem = ctx.importantItems.manta) && // and if we can't use manta
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
	};
}
namespace Modules {
	Hacks::AutoDodge AutoDodge{};
}