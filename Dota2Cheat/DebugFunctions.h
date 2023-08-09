#pragma once
#include "CheatSDK/include.h"

#ifdef _DEBUG

void LogEntities();
void LogModifiers(CDOTABaseNPC* npc);
void LogInvAndAbilities(CDOTABaseNPC* npc = nullptr);

#endif // _DEBUG
