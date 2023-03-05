#pragma once
#include "../../SDK/include.h"

namespace Hacks {
	// Heroes have the m_clrRender netvar which changes their color(like the blue friendly illusions)
	// The netvar is unique in having a callback
	class IllusionColoring {
	public:
		bool ColorIfIllusion(CBaseEntity* ent) {
			if (!Config::IllusionColoring ||
				!ctx.heroes.count((CDOTABaseNPC_Hero*)ent))
				return false;

			const char* className = ent->SchemaBinding()->binaryName;
			auto hero = (CDOTABaseNPC_Hero*)ent;
			if (ctx.assignedHero &&
				ctx.assignedHero->GetTeam() != hero->GetTeam() &&
				hero->IsIllusion() &&
				!strstr(className, "CDOTA_Unit_Hero_ArcWarden") //Arc's double is replicating his model but is not an illusion
				) {
				hero->SetColor(Color(Config::IllusionColor.x * 255, Config::IllusionColor.y * 255, Config::IllusionColor.z * 255));
				return true;
			}

			return false;
		}
	};
}

namespace Modules {
	inline Hacks::IllusionColoring IllusionColoring{};
}