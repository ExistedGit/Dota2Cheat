#pragma once
#include "../SDK/pch.h"
#include "../CheatSDK/include.h"


namespace Modules {
inline 
	// Uses Wand/Stick/Locket and/or FaerieFire at low health
	class AutoHeal {
		void UseWand(CDOTABaseNPC* hero);
		void UseFaerieFire(CDOTABaseNPC* hero);
	public:
		void OnFrame(CDOTABaseNPC* hero) {
			UseWand(hero);
			UseFaerieFire(hero);
		}
	} AutoHeal;
}