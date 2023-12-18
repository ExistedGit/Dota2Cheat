#pragma once
#include <map>

namespace Modules {
	inline class M_DotaPlusManager {
		// will update when injected
		bool updateSubscription = true;
		bool updateTiers = false; 

		std::map<int, int> heroXPCache;
	public:
		// rebuilt from xref: "Failed to find CDOTAGameAccountPlus"
		void UpdateDotaPlusStatus();
		void UpdateHeroTiers() {
			//if (!updateTiers)
			//	return;

			//if (Config::Changer::MaxHeroTiers)
			//	MaxHeroTiers();
			//else
			//	RestoreHeroTiers();
			//
		}
		void QueueUpdate() {
			updateSubscription = true;
		}

		// Set D+ hero tier XP
		// dota2 wiki has values for tiers
		void SetHeroXP(int heroId, int xp);

		void MaxHeroTiers() {
			for (int i = 1; i <= 124; i++) {
				//if(!heroXPCache.contains(i))
			}
		}

		void RestoreHeroTiers() {

		}
	} DotaPlusManager;
}