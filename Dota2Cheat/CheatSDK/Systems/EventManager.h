#pragma once
#include "../SDK/pch.h"
#include "../GameEventListeners/dota_player_update_assigned_hero.h"
#include "../GameEventListeners/map_shutdown.h"
#include "../GameEventListeners/dota_game_state_change.h"
#include "../GameEventListeners/LuaListener.h"

// Wrapper for CGameEventManager
inline class CEventManager {
	std::set<IGameEventListener2*> Listeners;
	std::set<LuaListener*> LuaListeners;
public:
	bool AddListener(IGameEventListener2* listener, const char* eventName) {
		Listeners.insert(listener);
		return (*GameSystems::GameEventManagerPtr)->AddListener(listener, eventName);
	}

	bool AddLuaListener(const sol::table& luaModule, const sol::function& func, const char* eventName) {
		auto listener = CMemAlloc::Instance()->AllocInit<LuaListener>();
		listener->func = func;
		listener->luaModule = luaModule;
		return AddListener(listener, eventName);
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

	// Lua listeners need to be cleared separately(in case the lua_state is reset, as it will invalidate the lua functions)
	void ClearLuaListeners() {
		for (auto& listener : LuaListeners)
			(*GameSystems::GameEventManagerPtr)->RemoveListener(listener);
	}

	void ClearListeners() {
		for (auto& listener : Listeners)
			(*GameSystems::GameEventManagerPtr)->RemoveListener(listener);
		for (auto& listener : LuaListeners)
			(*GameSystems::GameEventManagerPtr)->RemoveListener(listener);
		Listeners.clear();
		LuaListeners.clear();
	}
} EventManager;