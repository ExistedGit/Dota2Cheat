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
	for (auto& hero : ctx.heroes) {

		if (IsValidReadPtr(hero) &&
			IsValidReadPtr(hero->GetIdentity()) &&
			!hero->GetIdentity()->IsDormant())
			Modules::IllusionColoring.ColorIfIllusion(hero);
	}

	if (Config::AutoPickUpRunes) {

		for (auto& rune : ctx.runes) {
			if (!IsValidReadPtr(rune) ||
				!IsValidReadPtr(rune->GetIdentity()) ||
				rune->GetIdentity()->IsDormant())
				continue;
			Hooks::NetChan ? nullptr : throw "netchan = nullptr";
			// Morphling's snake_case technologies
			static long long last_pickup_time = 0;
			if (IsWithinRadius(rune->GetPos(), ctx.localHero->GetPos(), 140.0f) &&
				GetTickCount64() - last_pickup_time >= 200) {

				CDOTAClientMsg_ExecuteOrders orders_message;
				auto msg_id = Interfaces::NetworkMessages->FindNetworkMessageByID(350);
				auto order = orders_message.add_orders();
				order->set_order_type(DOTA_UNIT_ORDER_PICKUP_RUNE);
				order->set_target_index(rune->GetIndex());
				order->set_ability_index(0);
				order->set_sequence_number(ctx.localPlayer->GetSequenceNum() + 1);
				order->add_units(ctx.localHero->GetIndex());

				Hooks::oSendNetMessage(Hooks::NetChan, msg_id, &orders_message, BUF_DEFAULT);

				last_pickup_time = GetTickCount64();
			}
		}
	}
}

void InGameLogic() {

	Modules::AbilityESP.UpdateHeroData();
	//Modules::UIOverhaul.Update();

	UpdateCameraDistance();
	UpdateWeather();

	for (auto hero : ctx.heroes) {
		HeroData[hero].AbsOrigin = hero->GetPos();
		HeroData[hero].HealthbarW2S = WorldToScreen(hero->GetHealthBarPos());
	}

	if (!GameSystems::GameRules->IsGamePaused()) {

		Modules::TPTracker.FrameBasedLogic();
		Modules::BlinkRevealer.FrameBasedLogic();
		Modules::ParticleMaphack.FrameBasedLogic();

		if (ctx.localHero->GetLifeState() == 0) {
			Modules::AutoHeal.FrameBasedLogic(ctx.localHero);
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
	if (IsKeyPressed(VK_NUMPAD8)) {
		auto selected = ctx.localPlayer->GetSelectedUnits();
		auto ent = Interfaces::EntitySystem->GetEntity<CDOTABaseNPC>(selected[0]);
		auto pos = ent->GetPos();

		static Function init = Memory::Scan("E8 ? ? ? ? 8B C6 3B F3", "client.dll").GetAbsoluteAddress(1);
		auto& wearables = ctx.localHero->Field<CUtlVector<CHandle<CEconWearable>>>(Netvars::C_BaseCombatCharacter::m_hMyWearables);
		auto* item = wearables[0].Entity()->GetAttributeManager()->GetItem();
		init(item, Modules::SkinChanger.FakeItems.begin()->second, static_cast<BYTE>(0xFF));

		auto& oldWearables = ctx.localHero->Field<CUtlVector<CHandle<CEconWearable>>>(Netvars::C_DOTA_BaseNPC::m_hOldWearables);
		for (auto& handle : oldWearables)
			handle.val = 0;

		Log(LP_INFO, "OnWearablesChanged()");
		ent->OnWearablesChanged();
		//LogF(LP_INFO, "OnWearablesChanged(): {}\nResult: {}",
		//	(void*)ctx.localHero->GetVFunc(VTableIndexes::CDOTABaseNPC::OnWearablesChanged),
		//	ent->OnWearablesChanged());

		//auto buffs = ent->GetModifierManager()->GetBuffsByModifierFunction((ModifierFunction)62);
		//if (buffs)
		//	std::cout << std::dec << "ENT " << selected[0] << " -> " << ent
		//	<< "\n\t" << "POS " << pos.x << ' ' << pos.y << ' ' << pos.z
		//	// << "\n\tAttack Time: " << std::clamp(ent->GetBaseAttackTime() / ent->GetAttackSpeed(), 0.24f, 2.0f)
		//	//<< "\n\tIsRoshan: " << ent->IsRoshan()
		//	//<< "\n\tStunned: " << ent->HasState(ModifierState::MODIFIER_STATE_STUNNED)
		//	<< "\n\tValue:" << buffs->at(0).GetPropertyValue()
		//	<< '\n';
	}
	//if (GameSystems::ProjectileManager && IsKeyPressed(VK_NUMPAD3)) {
	//	auto arr = GameSystems::ProjectileManager->m_pTrackingProjectiles;
	//	std::cout << "[PROJECTILES]\n";
	//	for (int i = 0; i < 1024; i++) {
	//		auto proj = arr[i];
	//		if (!proj)
	//			continue;
	//		auto target = proj->GetTarget();
	//		auto source = proj->GetSource();
	//		std::cout << std::format("[{}] Move speed {} Source {} Target {} Dodgeable {} Attack {} Evaded {}\n",
	//			i,
	//			proj->GetMoveSpeed(),
	//			source ? source->GetUnitName() : "unknown",
	//			target ? target->GetUnitName() : "unknown",
	//			proj->IsDodgeable() ? "YES" : "NO",
	//			proj->IsAttack() ? "YES" : "NO",
	//			proj->IsEvaded() ? "YES" : "NO"
	//		);
	//	};
	//}
	if (IsKeyPressed(VK_RMENU)) {
		//auto type = GameSystems::GameRules->GetRiverType();
		//std::cout << "r: " << type << std::endl;

		//if (ctx.runes.size() > 0) {
		//	auto rune = *ctx.runes.begin();

		//	CDOTAClientMsg_ExecuteOrders orders_message;
		//	auto msg_id = Interfaces::NetworkMessages->FindNetworkMessageByID(350);
		//	auto order = orders_message.add_orders();
		//	order->set_order_type(DOTA_UNIT_ORDER_PICKUP_RUNE);
		//	order->set_target_index(rune->GetIndex());
		//	order->set_ability_index(0);
		//	order->set_sequence_number(ctx.localPlayer->GetSequenceNum() + 1);
		//	order->add_units(ctx.localHero->GetIndex());

		//	Hooks::oSendNetMessage(Hooks::NetChan, msg_id, &orders_message, BUF_DEFAULT);
		//}
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


	if (IsKeyPressed(VK_NUMPAD7)) {
		//auto wearables = ctx.localHero->MemberInline<CUtlVector<CHandle<>>>(Netvars::C_BaseCombatCharacter::m_hMyWearables);
		//LogF(LP_INFO, "Wearables: {}", (void*)wearables);
		//for (auto& handle : *wearables) {
		//	LogF(LP_DATA, "{} | {}", (void*)handle.Entity(), handle.Entity()->MemberInline<VClass>(0x960 + 0x68)->Member<uint32_t>(0x8));
		//};
	}
}
