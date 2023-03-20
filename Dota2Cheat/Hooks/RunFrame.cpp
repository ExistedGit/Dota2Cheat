#pragma once
#include "RunFrame.h"
#include <format>
#include "../Modules/Hacks/AbilityESP.h"

void Hooks::EntityIteration() {
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

void Hooks::UpdateCameraDistance() {
	static auto varInfo = CVarSystem::CVar["dota_camera_distance"];
	if (Config::CameraDistance != varInfo.var->value.flt) {
		varInfo.var->value.flt = Config::CameraDistance;
		Interfaces::CVar->TriggerCallback(varInfo);
	}
}

void Hooks::UpdateWeather() {
	static auto varInfo = CVarSystem::CVar["cl_weather"];
	varInfo.var->value.i32 = Config::WeatherListIdx;
}

void Hooks::hkRunFrame(uintptr_t a, uintptr_t b) {
	bool isInGame = Interfaces::Engine->IsInGame();

	if (!isInGame || ctx.gameStage != Context::GameStage::IN_GAME) {
		oRunFrame(a, b);
		return;
	}
	//std::cout << "frame\n";

	UpdateCameraDistance();
	UpdateWeather();

	Modules::AbilityESP.UpdateHeroData();
	if (
		ctx.assignedHero->GetLifeState() == 0 // if alive
		&& !GameSystems::GameRules->IsGamePaused() // and the game is not paused
		) { 
		AutoUseWandCheck(ctx.assignedHero, Config::AutoHealWandHPTreshold, Config::AutoHealWandMinCharges);
		AutoUseFaerieFireCheck(ctx.assignedHero, Config::AutoHealFaerieFireHPTreshold);

		Modules::AutoPing.FrameBasedLogic();
		Modules::AutoDodge.FrameBasedLogic();
		Modules::AutoBuyTome.FrameBasedLogic();
		Modules::RiverPaint.FrameBasedLogic();
		Modules::ParticleGC.FrameBasedLogic();
		Modules::TargetedSpellHighlighter.FrameBasedLogic();
		Modules::LinearProjectileWarner.FrameBasedLogic();
		EntityIteration();

		ctx.lua.safe_script("Modules.Core:EntityIteration()");
	}
#ifdef _DEBUG
	if (IsKeyPressed(VK_NUMPAD7)) {
		for (auto& hero : ctx.heroes)
			if (hero != ctx.assignedHero)
				std::cout << Modules::AttackAnimTracker.WillUnitAttack(ctx.assignedHero, hero, 0.5) << '\n';

	}
	if (IsKeyPressed(VK_NUMPAD8)) {
		auto selected = ctx.localPlayer->GetSelectedUnits();
		auto ent = Interfaces::EntitySystem->GetEntity<CDOTABaseNPC>(selected[0]);
		auto pos = ent->GetPos();

		std::cout << std::dec << "ENT " << selected[0] << " -> " << ent
			<< "\n\t" << "POS " << pos.x << ' ' << pos.y << ' ' << pos.z
			<< "\n\tAttack Time: " << std::clamp(ent->GetBaseAttackTime() / ent->GetAttackSpeed(), 0.24f, 2.0f)
			//<< "\n\tIsRoshan: " << ent->IsRoshan()
			//<< "\n\tStunned: " << ent->HasState(ModifierState::MODIFIER_STATE_STUNNED)
			<< "\n\tValue:" << ent->GetModifierManager()->GetBuffsByModifierFunction(MODIFIER_PROPERTY_LIFESTEAL_AMPLIFY_PERCENTAGE)->at(0).GetPropertyValue()
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

	oRunFrame(a, b);
}
