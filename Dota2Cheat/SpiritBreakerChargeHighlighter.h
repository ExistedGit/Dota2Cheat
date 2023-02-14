#pragma once
#include <vector>
#include "CUtlVector.h"
#include "Wrappers.h"
#include "CDOTAParticleManager.h"
#include "Globals.h"

namespace Hacks {
	//Tracks the size of the modifier list to see if a new one was added
	//Fully optimized to use the least possible amount of string comparisons
	class SBChargeHighlighter {
	private:
		struct ChargeTrackingInfo {
			int prevModifierCount = 0;
			DotaModifier* boundModifier = nullptr;
			CDOTAParticleManager::ParticleWrapper particleWrap{};
		};

		std::map<BaseNpc*, ChargeTrackingInfo> TrackedEntities{};
	public:
		inline void SubscribeEntity(BaseNpc* ent) {
			TrackedEntities[ent] = ChargeTrackingInfo{};
		}
		inline void UnsubscribeEntity(BaseNpc* ent) {
			TrackedEntities.erase(ent);
		}

		// Checks subscribed entities for a new modifier being added and if it's Baratrum's charge
		inline void FrameBasedLogic() {
			for (auto& pair : TrackedEntities) {
				auto ent = pair.first;
				auto& trackingInfo = pair.second;
				auto buffList = ent->GetModifierManager()->GetModifierListRaw();

				if (buffList->m_Size > trackingInfo.prevModifierCount) { // modifier added
					int newElementAmount = buffList->m_Size - trackingInfo.prevModifierCount;

					for (int i = buffList->m_Size - newElementAmount; i < buffList->m_Size; i++) {
						auto buffName = buffList->at(i)->GetName(); // checking the name of the new buff

						if (strstr(buffName, "modifier_spirit_breaker_charge_of_darkness_vision")) {
							trackingInfo.boundModifier = buffList->at(i);
							DrawChargeMark(ent);
						}
					}

				}
				else if (buffList->m_Size < trackingInfo.prevModifierCount && // some modifier removed
					trackingInfo.boundModifier) { // can't be the charge if there wasn't any
					bool chargeWasRemoved = true;
					for (int i = 0; i < buffList->m_Size; i++) {
						//As the buffList is a list of pointers we can take advantage of it
						if (buffList->at(i) == trackingInfo.boundModifier) {
							chargeWasRemoved = false;
							break;
						};
					}

					if (chargeWasRemoved) {
						trackingInfo.boundModifier = nullptr;
						Globals::ParticleManager->DestroyParticle(trackingInfo.particleWrap);
					}
				}
				trackingInfo.prevModifierCount = buffList->m_Size;
			}
		}
		inline void Reset() {
			TrackedEntities.clear();
		}

		inline void DrawChargeMark(BaseNpc* ent) {

			TrackedEntities[ent].particleWrap = Globals::ParticleManager->CreateParticle(
				"particles/units/heroes/hero_spirit_breaker/spirit_breaker_charge_target.vpcf",
				CDOTAParticleManager::ParticleAttachment_t::PATTACH_OVERHEAD_FOLLOW,
				ent
			);
			TrackedEntities[ent].particleWrap.particle
				->SetControlPoint(0, &Vector3::Zero)
				->SetControlPoint(1, &Vector3::Zero);

		}

	};

}
namespace Modules {
	inline Hacks::SBChargeHighlighter SBChargeHighlighter{};
}