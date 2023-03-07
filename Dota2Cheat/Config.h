#pragma once
#include "SDK/Base/Vector.h"

namespace Config {
	inline static bool AutoMidasEnabled = true;

	inline static bool AutoPickUpRunes = true;
	inline static bool AutoPickUpAegis = false;

	inline static bool AutoBuyTome = false;

	inline static bool CastRedirection = true;
	
	inline static bool AutoWandEnabled = true;
	inline static float AutoHealWandHPTreshold = 10.0f;
	inline static int AutoHealWandMinCharges = 5;
	inline static float AutoHealFaerieFireHPTreshold = 7.0f;

	inline static bool WarnLinearProjectiles = true;

	inline static bool ShowEnemyPointSpells = true;
	inline static bool ShowEnemyTargetedSpells = true;
	inline static bool ShowLinkenSphere = true;

	inline static int WeatherListIdx = 0;
	inline static int RiverListIdx = 0;

	inline static float CameraDistance = 1200.0f;

	inline static bool RenderAllParticles = true;

	inline static Vector IllusionColor{ 1,0,0 };
	inline static bool IllusionColoring = true;
	
	inline static int CircleRadius = 1200;
	inline static Vector CircleRGB{ 0,0,0 };

	inline static bool VBEShowText = true;
	inline static bool VBEShowParticle = true;
}