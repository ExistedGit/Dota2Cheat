#pragma once
#include "Wrappers.h"
#include "Config.h"

namespace Hacks {
	class ArmletAbuse {
		bool abuseInProgress = false;

		int frameCounter = 0;
		int expectedHP = 0;
	public:
		inline void Reset() {
			abuseInProgress = false;
		}

		inline void SwitchArmletIfNeeded(BaseNpc* ent) {
			float hpPercentage = (float)ent->GetHealth() / ent->GetMaxHealth();

			auto armlet = ent->FindItemBySubstring("armlet");
			if (!armlet.IsValid())
				return;

			auto item = armlet.GetAs <BaseAbility>();
			if (abuseInProgress
				&& item->IsToggled()
				&& ent->GetHealth() <= expectedHP) {
				frameCounter++;

				if (frameCounter >= 60* 0.6) {
					frameCounter = 0;
					abuseInProgress = false;
					expectedHP = 0;
				}
				else
					return;
			}
			if (abuseInProgress && hpPercentage <= Config::ArmletHPTreshold / 100)
				return;

			abuseInProgress = false;

			if (hpPercentage <= Config::ArmletHPTreshold / 100) {
				abuseInProgress = true;
				expectedHP = ent->GetHealth() + 50;
				localPlayer->PrepareOrder(
					DotaUnitOrder_t::DOTA_UNIT_ORDER_CAST_TOGGLE,
					0,
					&Vector3::Zero,
					H2IDX(armlet.handle),
					PlayerOrderIssuer_t::DOTA_ORDER_ISSUER_PASSED_UNIT_ONLY,
					ent);
				if (item->IsToggled()) { // if it was already toggled
					localPlayer->PrepareOrder(
						DotaUnitOrder_t::DOTA_UNIT_ORDER_CAST_TOGGLE,
						0,
						&Vector3::Zero,
						H2IDX(armlet.handle),
						PlayerOrderIssuer_t::DOTA_ORDER_ISSUER_PASSED_UNIT_ONLY,
						ent,
						true);
				};
			}
		}
	};

}
namespace Modules {
	Hacks::ArmletAbuse ArmletAbuse{};
}