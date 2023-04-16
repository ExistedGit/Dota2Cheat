#pragma once
#include "SDK/pch.h"

#include "Lua/LuaModules.h"
#include "Lua/LuaInitialization.h"
#include "EventListeners.h"
#include "Hooks/RunFrame.h"
#include "Hooks/ModifierEvents.h"

#include "Modules/Hacks/AutoBuyTome.h"
#include "Modules/Hacks/AbilityESP.h"
#include "Modules/Hacks/UIOverhaul.h"
#include "Modules/Hacks/AutoPick.h"
#include "Modules/Hacks/AutoPing.h"

#include "HookHelper.h"
void FillPlayerList();

// Now that the iteration is based on collections, the cheat does not retain the entity lists upon reinjection
void CacheAllEntities();

void OnUpdatedAssignedHero();
void UpdateAssignedHero();

void EnteredPreGame();
void EnteredInGame();
void LeftMatch();

void CheckMatchState();