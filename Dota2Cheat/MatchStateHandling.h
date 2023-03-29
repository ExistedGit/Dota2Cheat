#pragma once
#include "SDK/pch.h"

#include "EventListeners.h"
#include "DebugFunctions.h"
#include "Hooks/RunFrame.h"
#include "Lua/LuaInitialization.h"
#include "Hooks/ModifierEvents.h"

#include "Modules/Hacks/AutoBuyTome.h"
#include "Modules/Hacks/AbilityESP.h"
#include <format>
#include "Modules/Hacks/AutoPick.h"
#include "Modules/Hacks/AutoPing.h"

void FillPlayerList();

// Now that the iteration is based on collections, the cheat does not retain the entity lists upon reinjection
void ReinjectEntIteration();
#define GetGameSystem(global) GameSystems::##global = *GameSystems::## global ##Ptr

void OnUpdatedAssignedHero();

void UpdateAssignedHero();

void EnteredMatch();

void EnteredInGame();


void LeftMatch();
void CheckMatchState();