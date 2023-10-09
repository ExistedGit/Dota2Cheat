#pragma once
#include "../pch.h"
#include "../CheatSDK/include.h"
#include "../MListeners.h"

namespace Modules {
inline 
	// Uses Wand/Stick/Locket and/or FaerieFire at low health
	class M_AutoHeal : public IFrameListener {
		void UseWand(CDOTABaseNPC* hero);
		void UseFaerieFire(CDOTABaseNPC* hero);
	public:
		void OnFrame() override {
			UseWand(ctx.localHero);
			UseFaerieFire(ctx.localHero);
		}
	} AutoHeal;
}