#pragma once
#include "../../SDK/include.h"
#include "Config.h"

namespace Hacks {
	// STUB
	class ArmletAbuse {
	public:
		inline void SwitchArmletIfNeeded(CDOTABaseNPC* ent) {
			float hpPercentage = (float)ent->GetHealth() / ent->GetMaxHealth();

			auto armlet = ent->FindItemBySubstring("armlet");
			if (!armlet.IsValid())
				return;

			auto item = armlet.GetAs <CDOTABaseAbility>();
			
		}
	};

}
namespace Modules {
	Hacks::ArmletAbuse ArmletAbuse{};
}