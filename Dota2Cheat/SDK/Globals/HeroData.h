#pragma once
#include <map>
#include <string>
#include "../Entities/CDOTAItem.h"
#include "../Entities/CDOTAModifier.h"
#include "../Entities/CDOTABaseNPCHero.h"

struct _HeroData {
	std::map<std::string, CDOTAModifier*> Modifiers;
	std::map<std::string, CDOTAItem*> Items;
};

inline std::map<CDOTABaseNPC*, _HeroData> HeroData;

inline void ClearHeroData() {
	for (auto& [_, data] : HeroData) {
		data.Modifiers.clear();
		data.Items.clear();
	}
}