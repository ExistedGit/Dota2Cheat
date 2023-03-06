#pragma once
#include "../SDK/include.h"

#include "../Config.h"
#include "../Modules/Hacks/IllusionColoring.h"
#include "../Modules/Hacks/AegisAutoPickup.h"
#include "../Modules/Hacks/AutoBuyTome.h"
#include "../Modules/Hacks/VBE.h"	
#include "../Modules/Hacks/RiverPaint.h"
#include "../Modules/Hacks/AutoUseMidas.h"
#include "../Modules/Hacks/AutoUseMagicWand.h"
#include "../Modules/Hacks/ShakerAttackAnimFix.h"
#include "../Modules/Hacks/TargetedSpellHighlighter.h"
#include "../Modules/Hacks/LinearProjectileWarner.h"
#include "../Modules/Hacks/AutoPing.h"
#include "../Modules/Hacks/AutoDodge.h"

#include "../Modules/Utility/ParticleGC.h"

#include "VMT.h"
#include "../Input.h"
#include "../SDK/Entities/CDOTAItemRune.h"

namespace Hooks {
	template<typename T = CBaseEntity>
	std::set<T*> GetEntitiesByFilter(const std::vector<const char*>& filters) {
		std::set<T*> vec{};
		for (int i = 0; i < Interfaces::EntitySystem->GetHighestEntityIndex(); i++) {
			auto* ent = Interfaces::EntitySystem->GetEntity(i);
			if (!ent || ent->GetIdentity()->IsDormant())
				continue;
			//std::cout << ent->SchemaBinding() << '\n';
			const char* className = ent->SchemaBinding()->binaryName;
			if (className && TestStringFilters(className, filters))
				vec.insert((T*)ent);
		}
		return vec;
	};

	inline void EntityIteration() {
		bool midasUsed = false;
		bool runePickUp = false;

		for (auto& ent : ctx.entities) {
			if (ent->GetIdentity()->IsDormant())
				continue;

			const char* className = ent->SchemaBinding()->binaryName;
			if (!className)
				continue;

			if (!midasUsed && CanUseMidas() && strstr(className, "Creep")) {

				auto creep = (CDOTABaseNPC*)ent;

				//neutral prefixes because Wildwing Ripper and Dark Troll Warlord spawn a tornado and skeletons respectively
				//they have their summoner's name in them but not the word "neutral"
				static std::vector<const char*> filters = {
						"ranged",
						"flagbearer",
						"siege",
						"alpha_wolf",
						"centaur_khan",
						"neutral_dark_troll_warlord",
						"ursa_warrior",
						"ogre_magi",
						"satyr_hellcaller",
						"neutral_enraged_wildkin"
				};

				auto midasEnt = ctx.importantItems.midas;

				// If the creep is visible, not one of ours, is alive, is within Midas's radius and its name matches one of the filters
				if (
					creep->GetTeam() != ctx.assignedHero->GetTeam() &&
					creep->GetHealth() > 0 &&
					!creep->IsWaitingToSpawn() &&
					IsWithinRadius(creep->GetPos(), ctx.assignedHero->GetPos(), midasEnt->GetEffectiveCastRange()) &&
					TestStringFilters(creep->GetUnitName(), filters)
					) {
					midasUsed = true;
					ctx.localPlayer->PrepareOrder(DOTA_UNIT_ORDER_CAST_TARGET, ent->GetIndex(), &Vector::Zero, midasEnt->GetIndex(), DOTA_ORDER_ISSUER_PASSED_UNIT_ONLY, ctx.assignedHero);
				}
			}
			else if (Config::AutoPickUpRunes && !runePickUp && ctx.runes.count((CDOTAItemRune*)ent)) {
				auto* rune = (CDOTAItemRune*)ent;
				if (
					rune->GetRuneType() == DotaRunes::BOUNTY &&
					IsWithinRadius(rune->GetPos(), ctx.assignedHero->GetPos(), 150.0f)
					)
					ctx.localPlayer->PrepareOrder(DOTA_UNIT_ORDER_PICKUP_RUNE, ent->GetIndex(), &Vector::Zero, 0, DOTA_ORDER_ISSUER_HERO_ONLY, ctx.assignedHero, false, false);
			}
			else {

				if (Modules::IllusionColoring.ColorIfIllusion(ent)
					|| Modules::AegisAutoPickup.PickUpIfAegis(ent))
					continue;
				//sol::table luaModules = ctx.lua["Modules"];
				//for (auto& pair : luaModules) {
				//	sol::function callback = pair.second.as<sol::table>()["OnEntity"];
				//	if (callback.get_type() != sol::type::nil)
				//		callback(ent);
				//}
			}
		}
	}

	inline void UpdateCameraDistance() {
		static auto varInfo = CVarSystem::CVar["dota_camera_distance"];
		if (Config::CameraDistance != varInfo.var->value.flt) {
			varInfo.var->value.flt = Config::CameraDistance;
			Interfaces::CVar->TriggerCallback(varInfo);
		}
	}
	inline void UpdateWeather() {
		static auto varInfo = CVarSystem::CVar["cl_weather"];
		varInfo.var->value.i32 = Config::WeatherListIdx;
	}


	inline void hkRunFrame(u64 a, u64 b) {
		bool isInGame = Interfaces::Engine->IsInGame();

		if (isInGame) {
			//std::cout << "frame\n";
			if (ctx.gameStage == Context::GameStage::IN_GAME) {
				//sol::function entIter = ctx.lua["Modules"]["Core"]["EntityIteration"];
				//entIter();

				UpdateCameraDistance();
				UpdateWeather();

				if (ctx.assignedHero->GetLifeState() == 0) { // if alive
					AutoUseWandCheck(ctx.assignedHero, Config::AutoHealWandHPTreshold, Config::AutoHealWandMinCharges);
					AutoUseFaerieFireCheck(ctx.assignedHero, Config::AutoHealFaerieFireHPTreshold);

					Modules::AutoPing.FrameBasedLogic();
					Modules::AutoDodge.FrameBasedLogic();
					Modules::AutoBuyTome.FrameBasedLogic();
					Modules::VBE.FrameBasedLogic();
					Modules::RiverPaint.FrameBasedLogic();
					Modules::ParticleGC.FrameBasedLogic();
					Modules::TargetedSpellHighlighter.FrameBasedLogic();
					Modules::LinearProjectileWarner.FrameBasedLogic();
					EntityIteration();
				}
#ifdef _DEBUG
				if (IsKeyPressed(VK_NUMPAD8)) {
					auto selected = ctx.localPlayer->GetSelectedUnits();
					auto ent = Interfaces::EntitySystem->GetEntity<CDOTABaseNPC>(selected[0]);
					auto pos = ent->GetPos();

					std::cout << std::dec << "ENT " << selected[0] << " -> " << ent
						<< "\n\t" << "POS " << pos.x << ' ' << pos.y << ' ' << pos.z
						<< "\n\tAttack Time: " << std::clamp(ent->GetBaseAttackTime() / ent->GetAttackSpeed(), 0.24f, 2.0f)
						//<< "\n\tIsRoshan: " << ent->IsRoshan()
						//<< "\n\t" << AttackTargetFinder::GetAttackTarget(ent)
						<< '\n';
				}
				if (IsKeyPressed(VK_NUMPAD3)) {
					auto arr = GameSystems::ProjectileManager->GetTrackingProjectiles();
					std::cout << "[PROJECTILES]\n";
					for (int i = 0; i < arr.size(); i++) {
						auto proj = arr[i];
						if (!proj)
							continue;
						auto target = proj->GetTarget();
						auto source = proj->GetSource();
						std::cout << std::format("[{}] Move speed {} Source {} Target {} Dodgeable {} Attack {} Evaded {}\n",
							i,
							proj->GetMoveSpeed(),
							source ? source->GetUnitName() : "unknown",
							target ? target->GetUnitName() : "unknown",
							proj->IsDodgeable() ? "YES" : "NO",
							proj->IsAttack() ? "YES" : "NO",
							proj->IsEvaded() ? "YES" : "NO"
						);
					};
				}
				if (IsKeyPressed(VK_HOME)) {

				}
#endif
			}
		}
		VMTs::UIEngine->GetOriginalMethod<decltype(&hkRunFrame)>(6)(a, b);
	}

}