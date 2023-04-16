#pragma once
#include "RunFrame.h"
#include <format>

void Hooks::EntityIteration() {
	for (auto& hero : ctx.heroes) {

		if (IsValidReadPtr(hero) &&
			IsValidReadPtr(hero->GetIdentity()) &&
			!hero->GetIdentity()->IsDormant())
			Modules::IllusionColoring.ColorIfIllusion(hero);
	}
	if (Config::AutoPickUpRunes) {
		for (auto& rune : ctx.runes) {

			if (rune->GetRuneType() != DOTA_RUNE_BOUNTY ||
				!IsWithinRadius(rune->GetPos(), ctx.assignedHero->GetPos(), 140.0f)
				)
				continue;

			ctx.localPlayer->PrepareOrder(
				DOTA_UNIT_ORDER_PICKUP_RUNE,
				rune->GetIndex(),
				&Vector::Zero,
				0,
				DOTA_ORDER_ISSUER_HERO_ONLY,
				ctx.assignedHero,
				false,
				true);
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
	varInfo.var->value.i32 = Config::Changer::WeatherListIdx;
}

ENT_HANDLE currentQueryUnit = INVALID_HANDLE;
DOTA_GC_TEAM quTeam;
bool DotaPlusStatus = false;

void Hooks::hkRunFrame(void* thisptr) {
	bool isInGame = Interfaces::Engine->IsInGame();
	if (DotaPlusStatus != Config::Changer::UnlockDotaPlus) {
		DotaPlusStatus = Config::Changer::UnlockDotaPlus;
		Modules::DotaPlusUnlocker.UpdateDotaPlusStatus();
	}

	if (Modules::SkinChanger.ItemsCreated) {
		Modules::SkinChanger.ItemsCreated = false;

		for (auto& item : Modules::SkinChanger.itemsToCreate)
			Modules::SkinChanger.AddItem(item);

		Modules::SkinChanger.itemsToCreate.clear();
	}

	if (!isInGame ||
		!ctx.localPlayer ||
		!ctx.assignedHero ||
		ctx.gameStage != Context::GameStage::IN_GAME) {
		oRunFrame(thisptr);
		return;
	}
	//std::cout << "frame\n";

	UpdateCameraDistance();
	UpdateWeather();

	Modules::AbilityESP.UpdateHeroData();
	//Modules::UIOverhaul.Update();
	if (
		ctx.assignedHero->GetLifeState() == 0 // if alive
		&& !GameSystems::GameRules->IsGamePaused() // and the game is not paused
		) {


		Modules::TPTracker.CacheHeroIcons();

		Modules::TPTracker.FrameBasedLogic();
		Modules::AutoHeal.FrameBasedLogic(ctx.assignedHero);
		Modules::AutoPing.FrameBasedLogic();
		Modules::AutoDodge.FrameBasedLogic();
		Modules::AutoMidas.FrameBasedLogic();
		Modules::AutoBuyTome.FrameBasedLogic();
		Modules::AegisAutoPickup.FrameBasedLogic();

		Modules::RiverPaint.FrameBasedLogic();

		Modules::TargetedSpellHighlighter.FrameBasedLogic();
		Modules::LinearProjectileWarner.FrameBasedLogic();

		Modules::ParticleGC.FrameBasedLogic();
		// Changes selected unit's team to ours
		// looks funny
#ifdef _DEBUG
		auto queryUnit = ctx.localPlayer->Member<ENT_HANDLE>(Netvars::C_DOTAPlayerController::m_hQueryUnit);
		if (HVALID(currentQueryUnit) && queryUnit != currentQueryUnit) {
			auto unit = Interfaces::EntitySystem->GetEntity<CDOTABaseNPC_Hero>(H2IDX(currentQueryUnit));
			if (unit)
				unit->Field<DOTA_GC_TEAM>(Netvars::C_BaseEntity::m_iTeamNum) = quTeam;
		}
		if (HVALID(queryUnit))
		{
			auto unit = Interfaces::EntitySystem->GetEntity<CDOTABaseNPC_Hero>(H2IDX(queryUnit));
			if (unit) {
				std::cout << "Selected: " << unit << '\n';
				quTeam = unit->GetTeam();
				unit->Field<DOTA_GC_TEAM>(Netvars::C_BaseEntity::m_iTeamNum) = ctx.assignedHero->GetTeam();
				currentQueryUnit = queryUnit;
			}
		}
#endif // _DEBUG

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

	oRunFrame(thisptr);
}
