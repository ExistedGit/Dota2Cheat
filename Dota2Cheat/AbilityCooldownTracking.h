#pragma once
#include <map>
#include "Wrappers.h"
namespace Hacks {
	struct CooldownInfo {
		BaseNpc::ItemOrAbility ability;
		float lastActiveTime;
		float lastActiveCooldown;
		float currentCooldown;
	};
	inline std::map<BaseNpc*, CooldownInfo> CooldownTracker{};
}