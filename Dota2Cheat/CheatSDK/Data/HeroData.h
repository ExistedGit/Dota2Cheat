#pragma once
#include "../pch.h"
#include <imgui/imgui.h>

// Centralized storage for data about heroes
// Allows to optimize data update and access times

struct _HeroData {
	std::map<std::string, CDOTAModifier*> Modifiers;
	std::map<std::string, CDOTAItem*> Items;
	ImVec2 HealthbarW2S; // healthbar's screen pos
	ImVec2 W2S; // W2S(AbsOrigin)
	Vector AbsOrigin;
};

inline std::map<CDOTABaseNPC*, _HeroData> HeroData;

inline void ClearHeroData() {
	for (auto& [_, data] : HeroData) {
		data.Modifiers.clear();
		data.Items.clear();
	}
	HeroData.clear();
}