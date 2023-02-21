#pragma once
#include "Globals.h"
#include "Interfaces.h"
#include "Wrappers.h"
#include "EventListeners.h"
#include "DebugFunctions.h"
#include "Hooks/RunFrame.h"

#include "AutoBuyTome.h"
#include "SpiritBreakerChargeHighlighter.h"
#include <format>

inline void FillPlayerList() {
	auto vec = Globals::PlayerResource->GetVecTeamPlayerData();
	std::cout << "<PLAYERS>\n";
	for (int i = 0; i < vec.m_Size; i++) {
		//Sleep(800);
		auto idx = vec[i].GetPlayerSlot() + 1;
		if (idx < 1)
			continue;

		auto player = (DotaPlayer*)Interfaces::EntitySystem->GetEntity(idx);
		if (player == nullptr)
			continue;

		auto hero = (BaseNpc*)player->GetAssignedHero();
		//std::cout << idx << " " << player << ' ' << player->GetIdentity() << '\n';

		std::cout << "Player " << std::dec << idx << ": " << player;
		if (hero &&
			hero->GetUnitName()) {
			std::cout << "\n\t" << hero->GetUnitName() << " " << hero;
			ctx.heroes.push_back(hero);
		}
		std::cout << '\n';

		//players.push_back(player);
	}
}

inline void EnteredMatch() {
	Globals::GameRules = *Globals::GameRulesPtr;
	Globals::PlayerResource = *Globals::PlayerResourcePtr;
	//	Globals::ScriptVM = *Globals::ScriptVMPtr;
	Globals::ParticleManager = *Globals::ParticleManagerPtr;
	Globals::GameEventManager = *Globals::GameEventManagerPtr;

	GameState gameState = Globals::GameRules->GetGameState();
	if (gameState == DOTA_GAMERULES_PREGAME ||
		gameState == DOTA_GAMERULES_GAME_IN_PROGRESS) {

		ctx.localPlayer = (DotaPlayer*)Interfaces::EntitySystem->GetEntity(Interfaces::Engine->GetLocalPlayerSlot() + 1);
		if (ctx.localPlayer == nullptr)
			return;
		ctx.assignedHero = (BaseNpc*)Interfaces::EntitySystem->GetEntity(H2IDX(ctx.localPlayer->GetAssignedHeroHandle()));
		if (ctx.assignedHero == nullptr)
			return;

		FillPlayerList();

		Modules::ShakerAttackAnimFix.SubscribeEntity(ctx.assignedHero);
		for (auto& hero : ctx.heroes) {
			if (hero->GetTeam() == ctx.assignedHero->GetTeam()) {
				Modules::SBChargeHighlighter.SubscribeEntity(hero);
				Modules::VBE.SubscribeEntity(hero);
			}
		}
		Modules::AutoBuyTome.Init();
		std::cout << "Local Player: " << ctx.localPlayer
			<< "\n\t" << std::dec << "STEAM ID: " << ctx.localPlayer->GetSteamID()
			<< '\n';
		std::cout << "Assigned Hero: " << ctx.assignedHero << " " << ctx.assignedHero->GetUnitName() << '\n';

		Interfaces::CVar->SetConvars();

		auto roshanListener = new RoshanListener();
		roshanListener->gameStartTime = Globals::GameRules->GetGameTime();
		auto hurtListener = new EntityHurtListener();
		Globals::GameEventManager->AddListener(roshanListener, "dota_roshan_kill");
		Globals::GameEventManager->AddListener(hurtListener, "entity_hurt");

		VMTs::UIEngine = std::unique_ptr<VMT>(new VMT(Interfaces::UIEngine));
		VMTs::UIEngine->HookVM(Hooks::hkRunFrame, 6);
		VMTs::UIEngine->ApplyVMT();

		Globals::LogGlobals();
#ifdef _DEBUG
		Test::HookParticles();
#endif // _DEBUG

		ctx.IsInMatch = true;
		ctx.lua["assignedHero"] = ctx.assignedHero;
		ctx.lua["localPlayer"] = ctx.localPlayer;

		std::cout << "ENTERED MATCH\n"; 
	}
}
inline void LeftMatch() {
	ctx.IsInMatch = false;

	Modules::AutoBuyTome.Reset();
	Modules::SBChargeHighlighter.Reset();
	Modules::VBE.Reset();

	Globals::PlayerResource = nullptr;
	Globals::GameRules = nullptr;
	Globals::ScriptVM = nullptr;
	Globals::ParticleManager = nullptr;

	VMTs::NetChannel.reset();
	VMTs::UIEngine.reset();

	for (auto& listener : CGameEventManager::EventListeners)
		Globals::GameEventManager->RemoveListener(listener.get());
	CGameEventManager::EventListeners.clear();
	Globals::GameEventManager = nullptr;


	ctx.localPlayer = nullptr;
	ctx.assignedHero = nullptr;
	ctx.lua["assignedHero"] = nullptr;
	ctx.lua["localPlayer"] = nullptr;
#ifdef _DEBUG
	Test::partMap.clear();
#endif // _DEBUG
	ctx.players.clear();
	ctx.heroes.clear();

	std::cout << "LEFT MATCH\n";
}
inline void CheckMatchState() {
	if (Interfaces::Engine->IsInGame()) {
		if (!ctx.IsInMatch)
			EnteredMatch();
	}
	else if (ctx.IsInMatch) LeftMatch();
}