#pragma once
#include <vector>
#include "CUtlVector.h"
#include "Wrappers.h"
#include "CDOTAParticleManager.h"
#include "Globals.h"

//Tracks the size of the modifier list to see if a new one was added
namespace Hacks {
	class SBChargeHighlighter {
	public:
		struct ChargeTrackingInfo {
			int prevModifierCount = 0;
			CDOTAParticleManager::ParticleWrapper particle{};
		};

		std::map<BaseNpc*, ChargeTrackingInfo> TrackedEntities{};

		inline void SubscribeEntity(BaseNpc* ent) {
			TrackedEntities[ent] = ChargeTrackingInfo{};
		}

		inline void CheckEntitiesForCharge() {
			for (auto& pair : TrackedEntities) {
				auto ent = pair.first;
				auto& trackingInfo = pair.second;
				auto buffList = ent->GetModifierManager()->GetModifierListRaw();

				if (buffList->m_Size != trackingInfo.prevModifierCount) {
					auto buffName = buffList->last()->GetName(); // checking the name of the new buff
					if (strstr(buffName, "modifier_spirit_breaker_charge_of_darkness_vision")) {
						DrawChargeMark
					}

					trackingInfo.prevModifierCount = buffList->m_Size;
				}
			}
		}

		inline void DrawChargeMark(BaseNpc* ent) {
			TrackedEntities[ent].particle = Globals::ParticleManager->CreateParticle(
				"particles/units/heroes/hero_spirit_breaker/spirit_breaker_charge_target.vpcf",
				CDOTAParticleManager::ParticleAttachment_t::PATTACH_ABSORIGIN_FOLLOW,
				ent
			);
			
		}

	};
}