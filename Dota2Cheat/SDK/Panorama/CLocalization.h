#pragma once
#include "../Base/VClass.h"
#include "../Base/Definitions.h"

class CLocalization : public VClass {
public:
	// accepts Valve's localization strings, like "#dota_hud_error_ability_disabled_by_root"
	char8_t* GetLocalizedString(const char* str) {
		return CallVFunc<15, char8_t*>(str);
	}
};