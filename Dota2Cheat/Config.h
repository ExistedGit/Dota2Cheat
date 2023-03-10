#pragma once
#include "SDK/Base/Vector.h"

namespace Config {
	inline bool AutoMidasEnabled = true;

	inline bool AutoPickUpRunes = true;
	inline bool AutoPickUpAegis = false;

	inline bool AutoBuyTome = false;

	inline bool CastRedirection = true;
	
	inline bool AutoWandEnabled = true;
	inline float AutoHealWandHPTreshold = 10.0f;
	inline int AutoHealWandMinCharges = 5;
	inline float AutoHealFaerieFireHPTreshold = 7.0f;

	inline bool WarnLinearProjectiles = true;

	inline bool ShowEnemyPointSpells = true;
	inline bool ShowEnemyTargetedSpells = true;
	inline bool ShowLinkenSphere = true;

	inline int WeatherListIdx = 0;
	inline int RiverListIdx = 0;

	inline float CameraDistance = 1200.0f;

	inline bool RenderAllParticles = true;

	inline Vector IllusionColor{ 1,0,0 };
	inline bool IllusionColoring = true;
	
	inline int CircleRadius = 1200;
	inline Vector CircleRGB{ 0,0,0 };

	inline bool VBEShowText = true;
	inline bool VBEShowParticle = true;
}