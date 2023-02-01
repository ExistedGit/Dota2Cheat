#pragma once

namespace Config {
	inline bool AutoMidasEnabled = true;
	
	inline bool AutoRunePickupEnabled = true;

	inline bool AutoBuyTome = false;
	
	inline bool AutoWandEnabled = true;
	inline float AutoHealWandHPTreshold = 10.0f;
	inline int AutoHealWandMinCharges = 5;
	inline float AutoHealFaerieFireHPTreshold = 7.0f;

	inline float CameraDistance = 1200.0f;
	

	inline bool VBEShowText = true;
	inline bool VBEShowParticle = true;

	inline int OffsetX = 0; 
	inline int OffsetY = 0;
	inline int OffsetZ = 0;
}