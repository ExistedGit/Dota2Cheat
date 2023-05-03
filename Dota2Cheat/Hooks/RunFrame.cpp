#pragma once
#include "RunFrame.h"
#include <format>

void UpdateCameraDistance() {
	static auto varInfo = CVarSystem::CVars["dota_camera_distance"];
	if (Config::CameraDistance != varInfo.var->value.flt) {
		varInfo.var->value.flt = Config::CameraDistance;
		Interfaces::CVar->TriggerCallback(varInfo);
	}
}

void UpdateWeather() {
	static auto varInfo = CVarSystem::CVars["cl_weather"];
	varInfo.var->value.i32 = Config::Changer::WeatherListIdx;
}

void Hooks::EntityIteration() {
	for (auto& hero : ctx.heroes) {

		if (IsValidReadPtr(hero) &&
			IsValidReadPtr(hero->GetIdentity()) &&
			!hero->GetIdentity()->IsDormant())
			Modules::IllusionColoring.ColorIfIllusion(hero);
	}

	if (Config::AutoPickUpRunes) {
		Hooks::NetChan ? nullptr : throw "netchan = nullptr";

		for (auto& rune : ctx.runes) {
			if (!IsValidReadPtr(rune) ||
				!IsValidReadPtr(rune->GetIdentity()) ||
				rune->GetIdentity()->IsDormant())
				continue;
			// Morphling's snake_case technologies
			static long long last_pickup_time = 0;
			if (IsWithinRadius(rune->GetPos(), ctx.assignedHero->GetPos(), 140.0f) &&
				GetTickCount64() - last_pickup_time >= 200) {

				CDOTAClientMsg_ExecuteOrders orders_message;
				auto msg_id = Interfaces::NetworkMessages->FindNetworkMessageByID(350);
				auto order = orders_message.add_orders();
				order->set_order_type(DOTA_UNIT_ORDER_PICKUP_RUNE);
				order->set_target_index(rune->GetIndex());
				order->set_ability_index(0);
				order->set_sequence_number(ctx.localPlayer->GetSequenceNum() + 1);
				order->add_units(ctx.assignedHero->GetIndex());

				Hooks::oSendNetMessage(Hooks::NetChan, msg_id, &orders_message, BUF_DEFAULT);
				last_pickup_time = GetTickCount64();
			}
		}
	}
}

void Hooks::hkRunFrame(void* thisptr) {
	bool isInGame = Interfaces::Engine->IsInGame();
	static bool DotaPlusStatus = false;
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

	Modules::AbilityESP.UpdateHeroData();
	//	Modules::UIOverhaul.Update();

	UpdateCameraDistance();
	UpdateWeather();

	if (!GameSystems::GameRules->IsGamePaused()) {

		Modules::TPTracker.FrameBasedLogic();
		Modules::BlinkRevealer.FrameBasedLogic();
		Modules::ParticleMaphack.FrameBasedLogic();

		if (ctx.assignedHero->GetLifeState() == 0) {
			Modules::AutoHeal.FrameBasedLogic(ctx.assignedHero);
			Modules::AutoPing.FrameBasedLogic();
			Modules::AutoDodge.FrameBasedLogic();
			Modules::AutoMidas.FrameBasedLogic();
			Modules::AegisSnatcher.FrameBasedLogic();
		}

		Modules::RiverPaint.FrameBasedLogic();

		Modules::TargetedSpellHighlighter.FrameBasedLogic();
		Modules::LinearProjectileWarner.FrameBasedLogic();

		Modules::ParticleGC.FrameBasedLogic();

		EntityIteration();

		ctx.lua.safe_script("Modules.Core:EntityIteration()");
	}
#ifdef _DEBUG
	if (IsKeyPressed(VK_NUMPAD7)) {
		auto wearables = ctx.assignedHero->Member < CUtlVector<ENT_HANDLE>>(Netvars::C_BaseCombatCharacter::m_hMyWearables);
		for (auto& w : wearables) {
			Log(LP_NONE, (void*)Interfaces::EntitySystem->GetEntity(H2IDX(w)));
		};
	}
	if (IsKeyPressed(VK_NUMPAD8)) {
		auto selected = ctx.localPlayer->GetSelectedUnits();
		auto ent = Interfaces::EntitySystem->GetEntity<CDOTABaseNPC>(selected[0]);
		auto pos = ent->GetPos();

		std::cout << std::dec << "ENT " << selected[0] << " -> " << ent
			<< "\n\t" << "POS " << pos.x << ' ' << pos.y << ' ' << pos.z
			// << "\n\tAttack Time: " << std::clamp(ent->GetBaseAttackTime() / ent->GetAttackSpeed(), 0.24f, 2.0f)
			//<< "\n\tIsRoshan: " << ent->IsRoshan()
			//<< "\n\tStunned: " << ent->HasState(ModifierState::MODIFIER_STATE_STUNNED)
			//<< "\n\tValue:" << ent->GetModifierManager()->GetBuffsByModifierFunction(MODIFIER_PROPERTY_LIFESTEAL_AMPLIFY_PERCENTAGE)->at(0).GetPropertyValue()
			<< '\n';
	}
	if (IsKeyPressed(VK_NUMPAD3)) {
		auto arr = GameSystems::ProjectileManager->m_pTrackingProjectiles;
		std::cout << "[PROJECTILES]\n";
		for (int i = 0; i < 1024; i++) {
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
	if (IsKeyPressed(VK_RMENU)) {
		auto type = GameSystems::GameRules->GetRiverType();
		std::cout << "r: " << type << std::endl;

		if (ctx.runes.size() > 0) {
			auto rune = *ctx.runes.begin();

			CDOTAClientMsg_ExecuteOrders orders_message;
			auto msg_id = Interfaces::NetworkMessages->FindNetworkMessageByID(350);
			auto order = orders_message.add_orders();
			order->set_order_type(DOTA_UNIT_ORDER_PICKUP_RUNE);
			order->set_target_index(rune->GetIndex());
			order->set_ability_index(0);
			order->set_sequence_number(ctx.localPlayer->GetSequenceNum() + 1);
			order->add_units(ctx.assignedHero->GetIndex());

			Hooks::oSendNetMessage(Hooks::NetChan, msg_id, &orders_message, BUF_DEFAULT);
		}
	}
#endif 

	oRunFrame(thisptr);
}
