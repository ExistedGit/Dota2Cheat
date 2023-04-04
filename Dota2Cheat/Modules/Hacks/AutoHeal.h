#pragma once
#include "../SDK/pch.h"
#include "../Config.h"


namespace Hacks {
	// Uses Wand/Stick/Locket and/or FaerieFire at low health
	class AutoHeal {
		void UseWand(CDOTABaseNPC* hero);
		void UseFaerieFire(CDOTABaseNPC* hero);
	public:
		void FrameBasedLogic(CDOTABaseNPC* hero) {
			UseWand(hero);
			UseFaerieFire(hero);
		}
	};
}
namespace Modules {
	inline Hacks::AutoHeal AutoHeal{};
}