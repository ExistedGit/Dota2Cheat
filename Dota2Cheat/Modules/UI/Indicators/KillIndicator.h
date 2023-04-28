#pragma once
#include "../../../SDK/pch.h"
#include "../../../CheatSDK/Utils.h"
#include "../../../CheatSDK/DrawData.h"
#include "../../../CheatSDK/Config.h"
#include "../../../Utils/Drawing.h"
#include <functional>

namespace Hacks {
	class KillIndicator {
		struct NukeData {
			uint32_t idx = 0;
			bool isPure = false;
			NukeData() {};
			NukeData(uint32_t idx) : idx(idx) {};
			NukeData(uint32_t idx, bool isPure) : idx(idx), isPure(isPure) {};
		};
		NukeData curData;

		// Dataset provided by WolfGPT
		const std::map<std::string, NukeData> HeroNukes = {
			{ "npc_dota_hero_dragon_knight", 0 },
			{ "npc_dota_hero_huskar", 0 },
			{ "npc_dota_hero_mars", 0 },
			{ "npc_dota_hero_night_stalker", 0 },
			{ "npc_dota_hero_ogre_magi", 0 },
			{ "npc_dota_hero_nevermore", 0 },
			{ "npc_dota_hero_pudge", NukeData{ 0, true }},
			{ "npc_dota_hero_slardar", 1 },
			{ "npc_dota_hero_sven", 0 },
			{ "npc_dota_hero_tidehunter", 0 },
			{ "npc_dota_hero_tusk", 0 },
			{ "npc_dota_hero_undying", 0 },
			{ "npc_dota_hero_skeleton_king", 0 },
			{ "npc_dota_hero_antimage", 5 },
			{ "npc_dota_hero_bounty_hunter", 0 },
			{ "npc_dota_hero_gyrocopter", 1 },
			{ "npc_dota_hero_luna", 0 },
			{ "npc_dota_hero_phantom_lancer", 0 },
			{ "npc_dota_hero_shadow_fiend", 0 },
			{ "npc_dota_hero_sniper", 5 },
			{ "npc_dota_hero_spectre", 0 },
			{ "npc_dota_hero_ursa", 0 },
			{ "npc_dota_hero_vengefulspirit", 0 },
			{ "npc_dota_hero_weaver", 1 },
			{ "npc_dota_hero_crystal_maiden", 0 },
			{ "npc_dota_hero_death_prophet", 0 },
			// { "npc_dota_hero_invoker", NukeData{ 2, true }},
			{ "npc_dota_hero_leshrac", 2 },
			{ "npc_dota_hero_lich", 0 },
			{ "npc_dota_hero_lina", 5 },
			{ "npc_dota_hero_lion", 5 }
		};
		const std::map<std::string, std::function<int(CDOTABaseNPC*)>> CustomBehaviors = {
			{
			"npc_dota_hero_antimage",
			[this](CDOTABaseNPC* ent) -> int {
				auto manaVoid = ctx.assignedHero->GetAbility(curData.idx);
				auto dmgPerMana = manaVoid->GetLevelSpecialValueFor("mana_void_damage_per_mana");
				auto resist = ent->GetMagicalArmorValue();
				return ent->GetHealth() - ((ent->GetMaxMana() - ent->GetMana()) * dmgPerMana) * (1 - resist);
			}
			},
			{
			"npc_dota_hero_necrolyte",
			[this](CDOTABaseNPC* ent) -> int {
				auto ult = ctx.assignedHero->GetAbility(curData.idx);
				auto dmgPerHealth = ult->GetLevelSpecialValueFor("damage_per_health");
				auto resist = ent->GetMagicalArmorValue();
				return ent->GetHealth() - ((ent->GetMaxHealth() - ent->GetHealth()) * dmgPerHealth) * (1 - resist);
			}
			},
			{
			"npc_dota_hero_nevermore",
			[this](CDOTABaseNPC* ent) -> int {
				auto raze = ctx.assignedHero->GetAbility(curData.idx);
				auto razeDebuff = ent->GetModifier("modifier_nevermore_shadowraze_debuff");
				auto dmg = raze->GetLevelSpecialValueFor("shadowraze_damage");
				auto resist = ent->GetMagicalArmorValue();
				if (razeDebuff) {

					auto dmgPerStack = raze->GetLevelSpecialValueFor("stack_bonus_damage");
					auto stacks = razeDebuff->GetStackCount();
					return ent->GetHealth() - (dmg + stacks * dmgPerStack) * (1 - resist);
				}
				else
					return ent->GetHealth() - dmg * (1 - resist);
			}
			}
		};
		int DefaultBehavior(CDOTABaseNPC* ent) {
			auto nuke = ctx.assignedHero->GetAbility(curData.idx);
			auto dmg = nuke->GetLevelSpecialValueFor("damage");
			auto resist = ent->GetMagicalArmorValue();

			return curData.isPure
				? ent->GetHealth() - dmg
				: ent->GetHealth() - dmg * (1 - resist);
		}
		void DrawIndicatorFor(CDOTABaseNPC* ent);
		bool Initialized = false;
	public:
		void Init() {
			auto unitName = ctx.assignedHero->GetUnitName();
			if (HeroNukes.contains(unitName)) {
				curData = HeroNukes.at(unitName);
				Initialized = true;
			}
		}
		void Reset() {
			Initialized = false;
		}
		void Draw() {
			if (!Config::Indicators::Kill || !Initialized)
				return;

			for (auto& hero : ctx.heroes) {
				if (hero->IsSameTeam(ctx.assignedHero)
					|| !hero->IsTargetable()
					|| hero->IsIllusion())
					continue;
				DrawIndicatorFor(hero);
			}
		}

	};
}
namespace Modules {
	inline Hacks::KillIndicator KillIndicator{};
}