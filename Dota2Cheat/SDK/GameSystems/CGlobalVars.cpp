#include "CGlobalVars.h"
#include "../CheatSDK/Globals/Interfaces.h"

CGlobalVars* CGlobalVars::GetInstance() {
	return (CGlobalVars*)Interfaces::NetworkClientService->GetIGameClient()->GetGlobals();
}
