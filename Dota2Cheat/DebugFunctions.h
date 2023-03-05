#pragma once
#include "SDK/Entities/CDOTABaseNPC.h"
#include "SDK/Globals/Interfaces.h"
#include "SDK/Base/VMT.h"

#ifdef _DEBUG
inline void LogEntities() {
	for (int i = 0; i < Interfaces::EntitySystem->GetHighestEntityIndex(); i++) {
		auto* ent = Interfaces::EntitySystem->GetEntity(i);
		if (ent == nullptr)
			continue;
		//std::cout << ent->SchemaBinding() << '\n';
		const char* className = ent->SchemaBinding()->binaryName;
		if (
			className
			//&& strstr(className, "Rune")
			)
			std::cout << className << ' ' << i
			//<< " // " << ent->GetPos().x << ' ' << ent->GetPos().y
			<< " -> " << ent << '\n';
	}
}
inline void LogModifiers(CDOTABaseNPC* npc) {
	std::cout << "modifiers:\n";
	for (const auto& modifier : npc->GetModifierManager()->GetModifierList())
		std::cout << "\t" << modifier->GetName() << ' ' << modifier << '\n';
}
inline void LogInvAndAbilities(CDOTABaseNPC* npc = nullptr) {
	if (npc == nullptr)
		npc = ctx.assignedHero;

	std::cout << std::dec;
	std::cout << "abilities: " << '\n';
	for (const auto& ability : npc->GetAbilities()) {
		if (ability.name) {

			std::cout << '\t' << ability.name << " " << H2IDX(ability.handle)
				//<< " CD: " << ability.GetEnt()->GetCooldown() 
				//<< ' ' << std::dec << ability.GetEnt()->GetEffectiveCastRange()
				<< ' ' << ability.GetEnt();

			std::cout << '\n';

		}
	}
	std::cout << "inventory: " << '\n';
	for (const auto& item : npc->GetItems()) {
		if (item.name)
			std::cout << '\t' << item.name << " " << H2IDX(item.handle)
			<< ' ' << item.GetEnt()
			<< '\n';
	}
}

#endif // _DEBUG
