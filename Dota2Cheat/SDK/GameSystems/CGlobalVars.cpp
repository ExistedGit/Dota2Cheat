#include "CGlobalVars.h"
#include "../Globals/Interfaces.h"

CGlobalVars* CGlobalVars::GetInstance() {
	return (CGlobalVars*)Interfaces::NetworkClientService->GetIGameClient()->GetGlobals();
}
