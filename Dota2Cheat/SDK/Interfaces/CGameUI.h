#pragma once
#include "../Base/Definitions.h"
#include "../Base/VClass.h"
#include "../Enums.h"

class CGameUI : public VClass {
public:
	GETTER(DOTAGameUIState_t, GetUIState, 0x138);
};