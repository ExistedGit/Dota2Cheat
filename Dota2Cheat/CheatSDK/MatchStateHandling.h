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

// A dota cheat's perception of reality relies on the game's stage,
// as obviously we cannot do in-game things while we're still picking heroes
// As such, constant checks are required. Might have as well hooked a callback,
// but why waste time and space on another signature?

void LeftMatch();
void CheckMatchState();