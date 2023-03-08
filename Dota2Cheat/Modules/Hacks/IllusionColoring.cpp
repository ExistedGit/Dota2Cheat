#include "IllusionColoring.h"

bool Hacks::IllusionColoring::ColorIfIllusion(CBaseEntity* ent) {
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
