#pragma once
#include "../SDK/pch.h"

#include "Lua/LuaModules.h"
#include "Lua/LuaInitialization.h"

#include "../Hooks/RunFrame.h"
#include "../Hooks/ModifierEvents.h"
#include "../Hooks/GameEvents.h"

#include "../Modules/Hacks/AutoBuyTome.h"
#include "../Modules/UI/AbilityESP.h"
#include "../Modules/UI/UIOverhaul.h"
#include "../Modules/UI/Indicators/KillIndicator.h"
#include "../Modules/Hacks/AutoPick.h"
#include "../Modules/Hacks/AutoPing.h"


void FillPlayerList();

// Now that the iteration is based on collections, the cheat does not retain the entity lists upon reinjection
void CacheAllEntities();

void OnUpdatedAssignedHero();
void UpdateAssignedHero();

void EnteredPreGame();
void EnteredInGame();
void LeftMatch();

void CheckMatchState();