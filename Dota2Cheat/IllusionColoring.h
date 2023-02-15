#pragma once
#include "Wrappers.h"

namespace Hacks {
	// Heroes have the m_clrRender netvar which changes their color(like the blue friendly illusions)
	// The netvar is unique in having a callback
	class IllusionColoring {
	public:
		inline void ColorIfIllusion(BaseEntity* ent) {
			const char* className = ent->SchemaBinding()->binaryName;
			if (className != nullptr &&
				strstr(className, "C_DOTA_Unit_Hero") != nullptr) {
				auto hero = (BaseNpcHero*)ent;
				if (
					assignedHero->GetTeam() != hero->GetTeam() &&
					hero->IsIllusion() &&
					strstr(className, "CDOTA_Unit_Hero_ArcWarden") == nullptr //Arc's double is replicating his model but is not an illusion
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