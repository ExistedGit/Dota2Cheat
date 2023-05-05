#pragma once
#include "../SDK/pch.h"
#include <imgui/imgui.h>

// Centralized storage for data about heroes

struct _HeroData {
	std::map<std::string, CDOTAModifier*> Modifiers;
	std::map<std::string, CDOTAItem*> Items;
	ImVec2 HealthbarW2S; // healthbar's screen pos
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