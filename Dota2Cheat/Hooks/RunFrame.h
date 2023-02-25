#pragma once
#include "vtabler.h"

#include "../ConVar.h"
#include "../Config.h"
#include "../IllusionColoring.h"
#include "../AegisAutoPickup.h"
#include "../AutoBuyTome.h"
#include "../VBE.h"
#include "../SpiritBreakerChargeHighlighter.h"
#include "../RiverPaint.h"
#include "../AutoUseMidas.h"
#include "../AutoUseMagicWand.h"
#include "../ShakerAttackAnimFix.h"

#include "../ParticleGC.h"

#include "../Interfaces.h"
#include "VMT.h"

namespace Hooks {
	template<typename T = BaseEntity>
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
			if (className == nullptr)
				continue;

			if (!midasUsed && CanUseMidas(ctx.assignedHero) && strstr(className, "Creep")) {

				auto creep = (BaseNpc*)ent;

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
					IsWithinRadius(creep->GetPos2D(), ctx.assignedHero->GetPos2D(), midasEnt->GetEffectiveCastRange()) &&
					TestStringFilters(creep->GetUnitName(), filters)
					) {
					midasUsed = true;
					ctx.localPlayer->PrepareOrder(DOTA_UNIT_ORDER_CAST_TARGET, ent->GetIdentity()->GetEntIndex(), &Vector3::Zero, midasEnt->GetIdentity()->GetEntIndex(), DOTA_ORDER_ISSUER_PASSED_UNIT_ONLY, ctx.assignedHero);
				}
			}
			else if (Config::AutoPickUpRunes && !runePickUp && strstr(className, "C_DOTA_Item_Rune")) {
				auto* rune = (ItemRune*)ent;
				if (
					rune->GetRuneType() == DotaRunes::BOUNTY &&
					IsWithinRadius(rune->GetPos2D(), ctx.assignedHero->GetPos2D(), 150.0f)
					)
					ctx.localPlayer->PrepareOrder(DOTA_UNIT_ORDER_PICKUP_RUNE, ent->GetIdentity()->GetEntIndex(), &Vector3::Zero, 0, DOTA_ORDER_ISSUER_HERO_ONLY, ctx.assignedHero, false, false);
				
			}
			else {
				//sol::table luaModules = ctx.lua["Modules"];
				//for (auto& pair : luaModules) {
				//	sol::function callback = pair.second.as<sol::table>()["OnEntity"];
				//	if (callback.get_type() != sol::type::nil)
				//		callback(ent);
				//}
				Modules::IllusionColoring.ColorIfIllusion(ent);
				Modules::AegisAutoPickup.PickUpIfAegis(ent);
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
			if (ctx.IsInMatch) {
				//sol::function entIter = ctx.lua["Modules"]["Core"]["EntityIteration"];
				//entIter();

				UpdateCameraDistance();
				UpdateWeather();

				if (ctx.assignedHero->GetLifeState() == 0) { // if alive
					AutoUseWandCheck(ctx.assignedHero, Config::AutoHealWandHPTreshold, Config::AutoHealWandMinCharges);
					AutoUseFaerieFireCheck(ctx.assignedHero, Config::AutoHealFaerieFireHPTreshold);
					Modules::AutoBuyTome.FrameBasedLogic();
					Modules::VBE.FrameBasedLogic();
					Modules::SBChargeHighlighter.FrameBasedLogic();
					Modules::RiverPaint.FrameBasedLogic();
					Modules::ParticleGC.FrameBasedLogic();

					EntityIteration();
				}
#ifdef _DEBUG
				if (IsKeyPressed(VK_NUMPAD8)) {
					auto selected = ctx.localPlayer->GetSelectedUnits();
					auto ent = Interfaces::EntitySystem->GetEntity<BaseNpc>(selected[0]);
					auto pos = ent->GetPos();

					std::cout << std::dec << "ENT " << selected[0] << " -> " << ent
						<< "\n\t" << "POS " << pos.x << ' ' << pos.y << ' ' << pos.z
						<< "\n\tAttack Time: " << clamp(ent->GetBaseAttackTime() / ent->GetAttackSpeed(), 0.24, 2)
						//<< "\n\tIsRoshan: " << ent->IsRoshan()
						//<< "\n\t" << AttackTargetFinder::GetAttackTarget(ent)
						<< '\n';
				}
				if (IsKeyPressed(VK_NUMPAD3)) {


				}
				if (IsKeyPressed(VK_HOME)) {

				}
#endif
			}
		}
		VMTs::UIEngine->GetOriginalMethod<decltype(&hkRunFrame)>(6)(a, b);
	}

}