#pragma once
#include "../../SDK/include.h"

namespace Config {
	inline bool AutoDodge = false;
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

				auto target = Interfaces::EntitySystem->GetEntity<CDOTABaseNPC_Hero>(H2IDX(proj->GetTarget()));
				auto source = Interfaces::EntitySystem->GetEntity<CDOTABaseNPC_Hero>(H2IDX(proj->GetSource()));

				if (target != ctx.assignedHero ||
					source->GetTeam() == ctx.assignedHero->GetTeam())
					continue;

				//if (counterspell && counterspell->GetCooldown() == 0)
				//	useTime = 1.2f;

				auto pos = proj->GetPos();
				auto targetPos = target->GetPos();
				Vector predictedPos{};
				{
					auto deltaMove = proj->GetMoveSpeed() * 0.1f;
					auto deltaX = pos.x - (*(Vector*)&targetPos).x;
					auto deltaY = (*(Vector*)&targetPos).y - pos.y;

					float angle = atan2(deltaY, deltaX);
					float sine = sin(angle), cosine = -cos(angle);
					auto moveVec = Vector{ cosine * deltaMove, sine * deltaMove, 0 };
					predictedPos = pos + moveVec;
				}

				if (!IsWithinRadius(predictedPos, target->GetPos(), target->GetHullRadius()))
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
					usedItem->GetIdentity()->GetEntIndex(),
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