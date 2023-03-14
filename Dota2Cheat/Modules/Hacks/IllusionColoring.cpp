#include "IllusionColoring.h"

bool Hacks::IllusionColoring::ColorIfIllusion(CBaseEntity* ent) {
	if (!Config::IllusionColoring::Enabled ||
		!ctx.heroes.count((CDOTABaseNPC_Hero*)ent))
		return false;

	const char* className = ent->SchemaBinding()->binaryName;
	auto hero = (CDOTABaseNPC_Hero*)ent;
	if (ctx.assignedHero &&
		ctx.assignedHero->GetTeam() != hero->GetTeam() &&
		hero->IsIllusion() &&
		!strstr(className, "CDOTA_Unit_Hero_ArcWarden") //Arc's double is replicating his model but is not an illusion
		) {
		auto c = Config::IllusionColoring::Color;
		hero->SetColor(Color(c.x * 255, c.y * 255, c.z * 255));
		return true;
	}

	return false;
}
