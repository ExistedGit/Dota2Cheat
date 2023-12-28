#pragma once
#include "../pch.h"
#include "../GameEventListeners/dota_player_update_assigned_hero.h"
#include "../GameEventListeners/map_shutdown.h"
#include "../GameEventListeners/event_func.h"
#include "../GameEventListeners/dota_game_state_change.h"

// Wrapper for CGameEventManager
inline class CEventManager {
	std::set<IGameEventListener2*> Listeners;
public:
	bool AddListener(IGameEventListener2* listener, const char* eventName) {
		Listeners.insert(listener);
		return (*GameSystems::GameEventManagerPtr)->AddListener(listener, eventName);
	}

	void RemoveListener(IGameEventListener2* listener) {
		(*GameSystems::GameEventManagerPtr)->RemoveListener(listener);
	}

#define EVENT_SUB(name) auto name = CMemAlloc::Instance()->AllocInit<name ## _l>(); \
(*GameSystems::GameEventManagerPtr)->AddListener(name, #name); \
Listeners.insert(name);
#define LOG_EVENT(name) auto name##_logger = CMemAlloc::Instance()->AllocInit<event_func>(); \
name##_logger->SetFunc([](CGameEvent* ev) { LogF(LP_INFO, "[EVENT] {}", #name); }); \
(*GameSystems::GameEventManagerPtr)->AddListener(name##_logger, #name); \
Listeners.insert(name##_logger);

	void InstallListeners() {
		EVENT_SUB(dota_player_update_assigned_hero);
		EVENT_SUB(map_shutdown);
		EVENT_SUB(dota_game_state_change);

		LOG_EVENT(npc_spawned);
		LOG_EVENT(dota_on_hero_finish_spawn);
		LOG_EVENT(dota_player_update_hero_selection);
		LOG_EVENT(dota_player_update_assigned_hero);
	}

#undef EVENT_SUB

	void ClearListeners() {
		for (auto& listener : Listeners)
			(*GameSystems::GameEventManagerPtr)->RemoveListener(listener);
		Listeners.clear();
	}
} EventManager;