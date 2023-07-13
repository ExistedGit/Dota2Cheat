#pragma once
#include "RunFrame.h"
#include <format>
#include "../SDK/Interfaces/GC/CEconWearable.h"

template<typename T = CBaseEntity>
std::set<T*> GetEntitiesByFilter(const std::vector<const char*>& filters) {
	std::set<T*> vec{};
	for (int i = 0; i <= Interfaces::EntitySystem->GetHighestEntityIndex(); ++i) {
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

void UpdateCameraDistance() {
	static auto varInfo = CVarSystem::CVars["dota_camera_distance"];
	if (Config::CameraDistance != varInfo.m_pVar->value.flt) {
		varInfo.m_pVar->value.flt = Config::CameraDistance;
		Interfaces::CVar->TriggerCallback(varInfo);
	}
}

void UpdateWeather() {
	static auto varInfo = CVarSystem::CVars["cl_weather"];
	varInfo.m_pVar->value.i32 = Config::Changer::WeatherListIdx;
}

void EntityIteration() {
	//for (auto& hero : ctx.heroes) {

	//	if (IsValidReadPtr(hero) &&
	//		IsValidReadPtr(hero->GetIdentity()) &&
	//		!hero->GetIdentity()->IsDormant())
	//		Modules::IllusionESP.ColorIfIllusion(hero);
	//}

	// if (Config::AutoPickUpRunes) {
	//	for (auto& rune : ctx.runes) {
	//		if (!IsValidReadPtr(rune) ||
	//			!IsValidReadPtr(rune->GetIdentity()) ||
	//			rune->GetIdentity()->IsDormant())
	//			continue;
	//		Hooks::NetChan ? nullptr : throw "netchan = nullptr";
	//		// Morphling's snake_case technologies
	//		static long long last_pickup_time = 0;
	//		if (IsWithinRadius(rune->GetPos(), ctx.localHero->GetPos(), 140.0f) &&
	//			GetTickCount64() - last_pickup_time >= 200) {

	//			CDOTAClientMsg_ExecuteOrders orders_message;
	//			auto msg_id = Interfaces::NetworkMessages->FindNetworkMessageByID(350);
	//			auto order = orders_message.add_orders();
	//			order->set_order_type(DOTA_UNIT_ORDER_PICKUP_RUNE);
	//			order->set_target_index(rune->GetIndex());
	//			order->set_ability_index(0);
	//			order->set_sequence_number(ctx.localPlayer->GetSequenceNum() + 1);
	//			order->add_units(ctx.localHero->GetIndex());

	//			Hooks::oSendNetMessage(Hooks::NetChan, msg_id, &orders_message, BUF_DEFAULT);

	//			last_pickup_time = GetTickCount64();
	//		}
	//	}
	//}
}

void InGameLogic() {

	Modules::AbilityESP.UpdateHeroData();
	//Modules::UIOverhaul.Update();

	UpdateCameraDistance();
	UpdateWeather();
	Modules::TreeChanger.UpdateTreeModels();
	EntityList.ForEachOfType(EntityType::Hero, [](const auto& wrap) {
		auto hero = wrap.As<CDOTABaseNPC>();
	HeroData[hero].AbsOrigin = hero->GetPos();
	HeroData[hero].W2S = WorldToScreen(hero->GetPos());
	HeroData[hero].HealthbarW2S = WorldToScreen(hero->GetHealthBarPos());
		});

	static IRunFrameListener* PassiveListeners[] = {
		&Modules::RiverPaint,
		&Modules::IllusionESP
	};

	for (auto l : PassiveListeners)
		l->OnFrame();

	if (!GameSystems::GameRules->IsGamePaused()) {
		static IRunFrameListener* ActiveListeners[] = {
			&Modules::TPTracker,
			&Modules::BlinkRevealer,
			&Modules::ParticleMaphack,
			&Modules::LinearProjectileWarner,
			&Modules::ParticleGC,
			&Modules::TargetedSpellHighlighter,
		};

		for (auto l : ActiveListeners)
			l->OnFrame();

		if (ctx.localHero->GetLifeState() == 0) {
			Modules::AutoHeal.OnFrame();
			Modules::AutoPing.OnFrame();
			Modules::AutoDodge.OnFrame();
			Modules::AutoMidas.OnFrame();
			Modules::AegisSnatcher.OnFrame();
		}

		EntityIteration();
	}
#ifdef _DEBUG
	if (IsKeyPressed(VK_NUMPAD7))
		Log(LP_INFO, "Hero model: ", ctx.localHero->GetModelName());

	else if (IsKeyPressed(VK_NUMPAD8)) {
		// updateWearables = true;

		auto selected = ctx.localPlayer->GetSelectedUnits();
		auto ent = Interfaces::EntitySystem->GetEntity<CDOTABaseNPC>(selected[0]);
		auto pos = ent->GetPos();
		std::cout << "ENT " << selected[0] << " -> " << ent
			<< "\n\t" << "POS " << pos.x << ' ' << pos.y << ' ' << pos.z
			<< '\n';
	}
#endif 
}

void Hooks::hkRunFrame() {
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

	if (isInGame &&
		ctx.localHero &&
		ctx.gameStage == GameStage::IN_GAME)
		InGameLogic();

	//static float lastPingTime = 0;
	//if (GameSystems::GameRules->GetGameTime() - lastPingTime >= 1.) {
	//	lastPingTime = GameSystems::GameRules->GetGameTime();

	//	auto msg_id = Interfaces::NetworkMessages->FindNetworkMessageByID(303);
	//	CDOTAClientMsg_MapPing pingMsg{};
	//	auto loc = new CDOTAMsg_LocationPing;
	//	loc->set_x(0);
	//	loc->set_y(0);
	//	loc->set_target(-1);
	//	loc->set_type(0);
	//	loc->set_direct_ping(false);
	//	loc->set_ping_source(k_ePingSource_Wheel);
	//	pingMsg.set_allocated_location_ping(loc);
	//	oSendNetMessage(Hooks::NetChan, msg_id, &pingMsg, BUF_DEFAULT);
	//}
}
