#pragma once
#include "../SDK/pch.h"
#include "../CheatSDK/include.h"
#include "../Modules/Hacks/IllusionColoring.h"
#include "../Modules/Hacks/AegisSnatcher.h"
#include "../Modules/Hacks/AutoBuyTome.h"
#include "../Modules/Hacks/RiverPaint.h"
#include "../Modules/Hacks/AutoHeal.h"
#include "../Modules/Hacks/ShakerAttackAnimFix.h"
#include "../Modules/Hacks/TargetedSpellHighlighter.h"
#include "../Modules/Hacks/LinearProjectileWarner.h"
#include "../Modules/Hacks/AutoPing.h"
#include "../Modules/Hacks/AutoDodge.h"
#include "../Modules/Hacks/AutoMidas.h"
#include "../Modules/Hacks/DotaPlusUnlocker.h"
#include "../Modules/Hacks/SkinChanger.h"

#include "../Modules/UI/UIOverhaul.h"
#include "../Modules/UI/AbilityESP.h"
#include "../Modules/UI/ParticleMaphack.h"

#include "../Modules/Utility/ParticleGC.h"

#include "../SDK/Entities/CDOTAItemRune.h"

// CUIEngineSource2's RunFrame, well... processes and renders a UI frame.
// Dota2Cheat mainly operates on a frame-by-frame basis
// This is not a hook, but a listener that's called in RunFrame

namespace Hooks {
	void hkRunFrame();
}