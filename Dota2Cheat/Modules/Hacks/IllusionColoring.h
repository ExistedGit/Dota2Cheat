#pragma once
#include "../../SDK/pch.h"
#include "../../CheatSDK/Config.h"

namespace Hacks {
	// Heroes have the m_clrRender netvar which changes their color(like the blue friendly illusions)
	// The netvar is unique in having a callback
	class IllusionColoring {
	public:
		bool ColorIfIllusion(CBaseEntity* ent);
	};
}

namespace Modules {
	inline Hacks::IllusionColoring IllusionColoring{};
}