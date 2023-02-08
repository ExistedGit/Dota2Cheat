#pragma once
#include "Globals.h"
#include "Interfaces.h"
#include "Wrappers.h"
#include "Hooks.h"
#include "AutoBuyTome.h"
#include "EventListeners.h"
#include "SpiritBreakerChargeHighlighter.h"

extern bool IsInMatch;

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
		if (hero != nullptr &&
			hero->GetUnitName() != nullptr)
			std::cout << "\n\t" << hero->GetUnitName() << " " << hero;
		std::cout << '\n';
		players.push_back(player);
	}
}

inline void EnteredMatch() {
	Globals::GameRules = *Globals::GameRulesPtr;
	Globals::PlayerResource = *Globals::PlayerResourcePtr;
	Globals::ScriptVM = *Globals::ScriptVMPtr;
	Globals::ParticleManager = *Globals::ParticleManagerPtr;
	Globals::GameEventManager = *Globals::GameEventManagerPtr;

	GameState gameState = Globals::GameRules->GetGameState();
	if (gameState == GameState::DOTA_GAMERULES_PREGAME ||
		gameState == GameState::DOTA_GAMERULES_GAME_IN_PROGRESS) {

		localPlayer = (DotaPlayer*)Interfaces::EntitySystem->GetEntity(Interfaces::Engine->GetLocalPlayerSlot() + 1);
		if (localPlayer == nullptr)
			return;
		assignedHero = (BaseNpc*)Interfaces::EntitySystem->GetEntity(H2IDX(localPlayer->GetAssignedHeroHandle()));
		if (assignedHero == nullptr)
			return;

		Modules::SBChargeHighlighter.SubscribeEntity(assignedHero);

		Modules::AutoBuyTome.Init();
		std::cout << std::hex << "Local Player: " << localPlayer
			<< "\n\t" << std::dec << "STEAM ID: " << localPlayer->GetSteamID()
			<< '\n';
		std::cout << std::hex << "Assigned Hero: " << assignedHero << " " << assignedHero->GetUnitName() << '\n';
		IsInMatch = true;
		Interfaces::CVar->SetConvars();
		//FillPlayerList();


		auto ptr = new RoshanListener();
		ptr->gameStartTime = Globals::GameRules->GetGameTime();
		CGameEventManager::EventListeners.push_back(std::unique_ptr<RoshanListener>(ptr));
		Globals::GameEventManager->AddListener(ptr, "dota_roshan_kill", false);

		VMTs::UIEngine = std::unique_ptr<VMT>(new VMT(Interfaces::UIEngine));
		VMTs::UIEngine->HookVM(Hooks::RunFrame, 6);
		VMTs::UIEngine->ApplyVMT();

		Globals::LogGlobals();
		std::cout << "ENTERED MATCH\n";
	}
}
inline void LeftMatch() {
	IsInMatch = false;
	
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


	localPlayer = nullptr;
	assignedHero = nullptr;

	Modules::AutoBuyTome.Reset();
	Modules::SBChargeHighlighter.Reset();
	Modules::VBE.Reset();

	players.clear();
	
	std::cout << "LEFT MATCH\n";
}
inline void CheckMatchState() {
	if (Interfaces::Engine->IsInGame()) {
		if (!IsInMatch)
			EnteredMatch();
	}
	else if (IsInMatch) LeftMatch();
}