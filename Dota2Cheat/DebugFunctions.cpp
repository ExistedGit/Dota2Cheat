#include "DebugFunctions.h"

void LogEntities() {
	std::stringstream buf;
	for (int idx = 0; idx <= Interfaces::EntitySystem->GetHighestEntityIndex(); ++idx) {
		if (CBaseEntity* entity = Interfaces::EntitySystem->GetEntity(idx); (entity && IsValidReadPtr(entity))) {
			const auto schema_bind = entity->SchemaBinding();
			if (entity->GetIdentity()->GetName() && schema_bind && schema_bind->binaryName) {
				buf << (void*)entity << ", " << entity->GetIdentity()->GetName() << ", " << schema_bind->binaryName << std::endl;
			}
		}
	}

	std::cout << buf.str();
}

void LogModifiers(CDOTABaseNPC* npc) {
	Log(LP_NONE, "modifiers:");
	for (const auto& modifier : npc->GetModifierManager()->GetModifierList())
		LogF(LP_NONE, "\t{} | {} | {}", (void*)modifier, (void*)modifier->GetAbility(), modifier->GetName());
}

void LogInvAndAbilities(CDOTABaseNPC* npc) {
	if (npc == nullptr)
		npc = ctx.localHero;

	std::cout << std::dec;
	std::cout << "abilities:" << '\n';
	for (const auto& ability : npc->GetAbilities()) {
		if (ability->GetIdentity()->GetName()) {

			std::cout << '\t' << ability->GetIdentity()->GetName() << " " << ability->GetIndex()
				//<< " CD: " << ability.GetEnt()->GetCooldown() 
				//<< ' ' << std::dec << ability.GetEnt()->GetEffectiveCastRange()
				<< ' ' << ability;

			std::cout << '\n';

		}
	}
	std::cout << "inventory: " << '\n';
	for (const auto& item : npc->GetItems()) {
		if (item->GetIdentity()->GetName())
			std::cout << '\t' << item->GetIdentity()->GetName() << " " << item->GetIndex()
			<< ' ' << item
			<< '\n';
	}
}
