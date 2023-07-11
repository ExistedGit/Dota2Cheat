#pragma once
#include "../../SDK/pch.h"
#include "../../CheatSDK/include.h"

namespace Modules {

	inline class M_IllusionESP {
	public:
		bool frameDone = false;
		void ColorIfIllusion(CBaseEntity* ent);
	} IllusionESP{};
}