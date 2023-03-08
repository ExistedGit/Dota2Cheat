#pragma once
#include <map>
#include "../SDK/pch.h"
namespace Hacks {
	struct AbilityInfo {
		CDOTABaseAbility* ability{};
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
			EnemyAbilities[hero].reserve(6);
		}
	}
	std::string assetsPath = "C:\\Users\\user\\Documents\\Dota2Cheat\\assets\\abilityIcons\\";
	void FrameBasedLogic() {
		for (auto& hero : ctx.heroes) {
			if (hero->GetTeam() == ctx.assignedHero->GetTeam())
				continue;
			for (int i = 0; i < 5; i++) {
				auto ability = hero->GetAbilities()[i];
				if (EnemyAbilities[hero][i].ability == ability)
					continue;

				if (ability->IsHidden())
					continue;
				EnemyAbilities[hero][i] = AbilityInfo{
					.ability = ability,
					.iconPath = (assetsPath + ability->GetIdentity()->GetName()).c_str(),
					.lastActiveTime = GameSystems::GameRules->GetGameTime(),
					.lastActiveCooldown = ability->GetCooldown(),
					.currentCooldown = ability->GetCooldown()
				};
			}
		}
	}
}