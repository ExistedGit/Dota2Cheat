#pragma once
#include "../GameEventListeners/dota_player_update_assigned_hero.h"
#include "../GameEventListeners/dota_on_hero_finish_spawn.h"
#include "../GameEventListeners/map_shutdown.h"
#include "../GameEventListeners/event_func.h"
#include "../GameEventListeners/game_rules_state_change.h"
#include "../GameEventListeners/dota_game_state_change.h"
#include "../GameEventListeners/lobby_updated.h"

// Wrapper for CGameEventManager
inline class CEventManager {
	std::set<IGameEventListener2*> Listeners;
public:
	bool AddListener(IGameEventListener2* listener, const char* eventName) {
		Listeners.insert(listener);
		return CEventMgr::Get()->AddListener(listener, eventName);
	}

	void RemoveListener(IGameEventListener2* listener) {
		CEventMgr::Get()->RemoveListener(listener);
	}

#define EVENT_SUB(name) auto name = CMemAlloc::Get()->AllocInit<name ## _l>(); \
CEventMgr::Get()->AddListener(name, #name); \
Listeners.insert(name);

#define LOG_EVENT(name) auto name##_logger = CMemAlloc::Get()->AllocInit<event_func>(); \
name##_logger->SetFunc([](CGameEvent* ev) { LogFI("[EVENT] {}", #name); }); \
CEventMgr::Get()->AddListener(name##_logger, #name); \
Listeners.insert(name##_logger);

	void InstallListeners() {
		EVENT_SUB(dota_player_update_assigned_hero);
		EVENT_SUB(map_shutdown);
		EVENT_SUB(lobby_updated);
		EVENT_SUB(dota_on_hero_finish_spawn);
		EVENT_SUB(dota_game_state_change);
		EVENT_SUB(game_rules_state_change);

#ifdef _DEBUG
		//LOG_EVENT(npc_spawned);
		LOG_EVENT(dota_player_update_hero_selection);
		LOG_EVENT(dota_player_update_assigned_hero);
		LOG_EVENT(dota_on_hero_finish_spawn);
		LOG_EVENT(dota_player_spawned);
#endif _DEBUG
	}

#undef EVENT_SUB

	void ClearListeners() {
		for (auto listener : Listeners)
			CEventMgr::Get()->RemoveListener(listener);
		Listeners.clear();
	}
} EventManager;