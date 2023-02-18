#pragma once
#include "Wrappers.h"
#include "Config.h"

namespace Hacks {
	// STUB
	class ArmletAbuse {
	public:
		inline void SwitchArmletIfNeeded(BaseNpc* ent) {
			float hpPercentage = (float)ent->GetHealth() / ent->GetMaxHealth();

			auto armlet = ent->FindItemBySubstring("armlet");
			if (!armlet.IsValid())
				return;

			auto item = armlet.GetAs <BaseAbility>();
			
		}
	};

}
namespace Modules {
	Hacks::ArmletAbuse ArmletAbuse{};
}