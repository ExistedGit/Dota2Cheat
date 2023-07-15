#include "CGlobalVars.h"
#include "../CheatSDK/Globals/Interfaces.h"

CGlobalVars* CGlobalVars::GetInstance() {
	return Interfaces::NetworkClientService->GetIGameClient()->GetGlobals();
}
