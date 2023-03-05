#pragma once
#include <map>
#include "../SDK/include.h"
namespace Hacks {
	struct AbilityInfo {
		ItemOrAbility ability{};
		const char* iconPath{};

		float lastActiveTime{};
		float lastActiveCooldown{};
		float currentCooldown{};
	};

	inline std::map<CDOTABaseNPC*, std::vector<AbilityInfo>> EnemyAbilities{};

	void Init() {
		for (auto& hero : ctx.heroes) {
			if (hero->GetTeam() == ctx.assignedHero->GetTeam())
				continue;
			EnemyAbilities[hero].reserve(5);
		}
	}
	std::string assetsPath = "C:\\Users\\user\\Documents\\Dota2Cheat\\assets\\abilityIcons\\";
	void FrameBasedLogic() {
		for (auto& hero : ctx.heroes) {
			if (hero->GetTeam() == ctx.assignedHero->GetTeam())
				continue;
			for (int i = 0; i < 5; i++) {
				auto ability = hero->GetAbilities()[i];
				if (EnemyAbilities[hero][i].ability.handle == ability.handle)
					continue;

				if (ability.GetEnt()->IsHidden())
					continue;
				EnemyAbilities[hero][i] = AbilityInfo{
					.ability = ability,
					.iconPath = (assetsPath + ability.name).c_str(),
					.lastActiveTime = GameSystems::GameRules->GetGameTime(),
					.lastActiveCooldown = ability.GetEnt()->GetCooldown(),
					.currentCooldown = ability.GetEnt()->GetCooldown()
				};
			}
		}
	}
}