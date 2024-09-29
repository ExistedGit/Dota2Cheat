#include "Debug.h"

void LogEntities() {
	std::stringstream buf;
	for (int idx = 0; idx <= CEntSys::Get()->GetHighestEntityIndex(); ++idx) {
		if (CBaseEntity* entity = CEntSys::Get()->GetEntity(idx); (entity && IsValidReadPtr(entity))) {
			const auto schema_bind = entity->SchemaBinding();
			if (entity->GetIdentity()->GetName() && schema_bind && schema_bind->binaryName) {
				buf << (void*)entity << ", " << entity->GetIdentity()->GetName() << ", " << schema_bind->binaryName << std::endl;
			}
		}
	}

	std::cout << buf.str();
}

void LogModifiers(CDOTABaseNPC* npc) {
	Log("Modifiers:");
	for (const auto& modifier : npc->GetModifierManager()->GetModifierList())
		LogF("\t{} | {} | {}", (void*)modifier, (void*)modifier->GetAbility(), modifier->GetName());
}

void LogInvAndAbilities(CDOTABaseNPC* npc) {
	if (npc == nullptr)
		npc = ctx.localHero;

	std::lock_guard _lock(mLogging);

	std::cout << std::dec;
	std::cout << "abilities:" << '\n';
	for (auto ability : npc->GetAbilities()) {
		if (!ability || !ability->GetIdentity()->GetName())
			continue;

		std::cout << '\t' << ability->GetIdentity()->GetName() << " " << ability->GetIndex()
			<< ' ' << ability << std::endl;
	}

	std::cout << "inventory: " << '\n';
	for (const auto& item : npc->GetItems()) {
		if (item && item->GetIdentity()->GetName())
			std::cout << '\t' << item->GetIdentity()->GetName() << " " << item->GetIndex()
			<< ' ' << *item
			<< std::endl;
	}
}
