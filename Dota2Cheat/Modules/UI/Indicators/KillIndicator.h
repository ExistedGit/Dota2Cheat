#pragma once
#include "../../../CheatSDK/include.h"
#include "../../../Utils/Drawing.h"

#include "../../MListeners.h"
#include "../MultiThreadModule.h"
#include <functional>

namespace Modules {
	inline class M_KillIndicator : public MultiThreadModule, public IFrameListener, public IEntityListListener {
		
		struct CalculationBehaviors {
			// npc = target of cast
			
			static int Default(const CAbility* nuke, const CNPC* npc) {
				return nuke->GetLevelSpecialValueFor("damage");
			}

			// nuke = Mana Void
			static int AntiMage(const CAbility* nuke, const CNPC* npc) {
				auto dmgPerMana = nuke->GetLevelSpecialValueFor("mana_void_damage_per_mana");
				return (npc->GetMaxMana() - npc->GetMana()) * dmgPerMana;
			}

			// nuke = Shadowraze
			static int ShadowFiend(const CAbility* nuke, const CNPC* npc) {
				auto razeDebuff = HeroData[(CHero*)npc].Modifiers["modifier_nevermore_shadowraze_debuff"];
				auto dmg = nuke->GetLevelSpecialValueFor("shadowraze_damage");
				auto dmgTotal = dmg;

				if (razeDebuff) {
					auto dmgPerStack = nuke->GetLevelSpecialValueFor("stack_bonus_damage");
					auto stacks = razeDebuff->GetStackCount();
					dmgTotal = dmg + stacks * dmgPerStack;
				}

				return dmgTotal;
			}

			// nuke = Scythe
			static int Necrolyte(const CAbility* nuke, const CNPC* npc) {
				auto dmgPerHealth = nuke->GetLevelSpecialValueFor("damage_per_health");
				auto dmgTotal = (npc->GetMaxHealth() - npc->GetHealth()) * dmgPerHealth;
				return dmgTotal;
			}

			// nuke = Finger of Death
			static int Lion(const CAbility* nuke, const CNPC* npc) {
				auto killCounter = HeroData[ctx.localHero].Modifiers["modifier_lion_finger_of_death_kill_counter"];
				auto dmg = nuke->GetLevelSpecialValueFor("damage");
				auto dmgTotal = dmg;

				if (killCounter) {
					auto dmgPerStack = nuke->GetLevelSpecialValueFor("damage_per_kill");
					auto stacks = killCounter->GetStackCount();
					dmgTotal = dmg + stacks * dmgPerStack;
				}

				return dmgTotal;
			}
		};

		struct NukeData {
			uint32_t idx = 0;
			bool isPure = false;
			NukeData() {};
			NukeData(uint32_t idx) : idx(idx) {};
			NukeData(uint32_t idx, bool isPure) : idx(idx), isPure(isPure) {};
		};
		NukeData curData;

		// Includes magical resistance and newly added barriers
		int CalcDmgWithResists(CNPC* npc, float dmg, bool pure = false) {
			auto barriers = npc->GetBarriers();
			auto effectiveDamage = dmg - (barriers.all + barriers.magic);
			if (!pure)
				effectiveDamage *= (1 - npc->GetMagicalArmorValue());

			return npc->GetHealth() - effectiveDamage;
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

		using fnCalculationBehavior = std::function<int(const CAbility* nuke, const CNPC* hero)>;

		const std::unordered_map<std::string, fnCalculationBehavior> CustomBehaviors = {
			{ "npc_dota_hero_antimage", CalculationBehaviors::AntiMage },
			{ "npc_dota_hero_necrolyte", CalculationBehaviors::Necrolyte },
			{ "npc_dota_hero_nevermore", CalculationBehaviors::ShadowFiend },
			{ "npc_dota_hero_lion", CalculationBehaviors::Lion },
		};

		bool Initialized = false;
	public:
		void Init() {
			MTM_LOCK;

			auto unitName = ctx.localHero->GetUnitName();
			if (HeroNukes.contains(unitName)) {
				curData = HeroNukes.at(unitName);
			}

			Initialized = true;
		}
		struct RenderData {
			bool drawable = false;
			int diff;
			ImVec2 pos;
		};

		const char* nukeIcon = nullptr;

		std::map<entidx_t, RenderData> renderData;

		void Reset() {
			Initialized = false;
		}

		void Draw();

		M_KillIndicator() : IFrameListener() {

			IEntityListListener::Subscribe(this);
		}

		// Inherited via IFrameListener
		void OnFrame() override;

		// Inherited via IEntityListListener
		void OnEntityAdded(const EntityWrapper& npc) override;
		void OnEntityRemoved(const EntityWrapper& npc) override;
	} KillIndicator;
}