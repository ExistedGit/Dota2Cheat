#pragma once
#include "Wrappers.h"

namespace Hacks {
	class IllusionColoring {
	public:
		inline void ColorIfIllusion(BaseEntity* ent) {
			const char* className = ent->SchemaBinding()->binaryName;
			if (className != nullptr &&
				strstr(className, "C_DOTA_Unit_Hero") != nullptr) {
				auto hero = (BaseNpcHero*)ent;
				if (
					hero->IsIllusion() &&
					strstr(className, "CDOTA_Unit_Hero_ArcWarden") == nullptr && //Arc's double is replicating his model but is not an illusion
					assignedHero->GetTeam() != hero->GetTeam()
					) {

					hero->SetColor(Color(Config::IllusionColor.x * 255, Config::IllusionColor.y * 255, Config::IllusionColor.z * 255));
				}
			}
		}
	};
}

namespace Modules {
	Hacks::IllusionColoring IllusionColoring{};
}