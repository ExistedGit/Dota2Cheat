#pragma once
#include "../pch.h"

// Centralized storage for data about heroes
// Allows to optimize data update and access times

struct _HeroData {
	std::unordered_map<std::string, CDOTAModifier*> Modifiers;
	std::unordered_map<std::string, CDOTAItem*> Items;
	ImVec2 HealthbarW2S; // healthbar's screen pos
	ImVec2 W2S; // W2S(AbsOrigin)
	Vector AbsOrigin;
};

inline std::unordered_map<CDOTABaseNPC*, _HeroData> HeroData;