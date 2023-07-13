#pragma once
#include "../../../pch.h"
#include "../../../CheatSDK/include.h"
#include "../../../Utils/Drawing.h"
#include "../MultiThreadModule.h"
#include <functional>

namespace Modules {
	inline class KillIndicator : public MultiThreadModule {
		struct NukeData {
			uint32_t idx = 0;
			bool isPure = false;
			NukeData() {};
			NukeData(uint32_t idx) : idx(idx) {};
			NukeData(uint32_t idx, bool isPure) : idx(idx), isPure(isPure) {};
		};
		NukeData curData;

		// Includes magical resistance and newly added barriers
		int CalcDmgWithResists(CDOTABaseNPC* ent, float dmg, bool pure = false) {
			auto barriers = ent->GetBarriers();
			auto effectiveDamage = dmg - (barriers.all + barriers.magic);
			if (!pure)
				effectiveDamage *= (1 - ent->GetMagicalArmorValue());

			return ent->GetHealth() - effectiveDamage;
		}

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
			// { "npc_dota_hero_weaver", 1 },
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
				static auto ult = ctx.localHero->GetAbility(curData.idx);
				auto dmgPerMana = ult->GetLevelSpecialValueFor("mana_void_damage_per_mana");
				return CalcDmgWithResists(ent, (ent->GetMaxMana() - ent->GetMana()) * dmgPerMana);
			}
			},
			{
			"npc_dota_hero_necrolyte",
			[this](CDOTABaseNPC* ent) -> int {
				static auto ult = ctx.localHero->GetAbility(curData.idx);
				auto dmgPerHealth = ult->GetLevelSpecialValueFor("damage_per_health");
				auto dmgTotal = (ent->GetMaxHealth() - ent->GetHealth()) * dmgPerHealth;
				return CalcDmgWithResists(ent, dmgTotal);
			}
			},
			{
			"npc_dota_hero_nevermore",
			[this](CDOTABaseNPC* ent) -> int {
				static auto raze = ctx.localHero->GetAbility(curData.idx);
				auto razeDebuff = HeroData[ent].Modifiers["modifier_nevermore_shadowraze_debuff"];
				auto dmg = raze->GetLevelSpecialValueFor("shadowraze_damage");
				auto dmgTotal = dmg;

				if (razeDebuff) {
					auto dmgPerStack = raze->GetLevelSpecialValueFor("stack_bonus_damage");
					auto stacks = razeDebuff->GetStackCount();
					dmgTotal = dmg + stacks * dmgPerStack;
				}

				return CalcDmgWithResists(ent, dmgTotal);
			}
			},
			{
			"npc_dota_hero_lion",
			[this](CDOTABaseNPC* ent) -> int {
				static auto ult = ctx.localHero->GetAbility(curData.idx);

				auto killCounter = HeroData[ctx.localHero].Modifiers["modifier_lion_finger_of_death_kill_counter"];
				auto dmg = ult->GetLevelSpecialValueFor("damage");
				auto dmgTotal = dmg;

				if (killCounter) {
					auto dmgPerStack = ult->GetLevelSpecialValueFor("damage_per_kill");
					auto stacks = killCounter->GetStackCount();
					dmgTotal = dmg + stacks * dmgPerStack;
				}

				return CalcDmgWithResists(ent, dmgTotal);
			}
			}
		};
		int DefaultBehavior(CDOTABaseNPC* ent) {
			auto nuke = ctx.localHero->GetAbility(curData.idx);
			auto dmg = nuke->GetLevelSpecialValueFor("damage");

			return CalcDmgWithResists(ent, dmg, curData.isPure);
		}
		void DrawIndicatorFor(CDOTABaseNPC_Hero* ent);
		bool Initialized = false;
	public:
		void Init() {
			MTM_LOCK;

			auto unitName = ctx.localHero->GetUnitName();
			if (HeroNukes.contains(unitName)) {
				curData = HeroNukes.at(unitName);
				Initialized = true;
			}
		}
		void Reset() {
			Initialized = false;
		}
		void Draw() {
			MTM_LOCK;

			if (!Config::Indicators::Kill || !Initialized)
				return;

			EntityList.ForEach<CDOTABaseNPC_Hero>([this](auto hero) { DrawIndicatorFor(hero); });
		}

	} KillIndicator;
}