#pragma once
#include "../pch.h"
#include "../GameEventListeners/dota_player_update_assigned_hero.h"
#include "../GameEventListeners/map_shutdown.h"
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

	void InstallListeners() {
		EVENT_SUB(dota_player_update_assigned_hero);
		EVENT_SUB(map_shutdown);
		EVENT_SUB(dota_game_state_change);
	}

#undef EVENT_SUB

	void ClearListeners() {
		for (auto& listener : Listeners)
			(*GameSystems::GameEventManagerPtr)->RemoveListener(listener);
		Listeners.clear();
	}
} EventManager;