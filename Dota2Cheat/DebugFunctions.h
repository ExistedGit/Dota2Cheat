#pragma once
#include "Wrappers.h"
#include "Interfaces.h"

#ifdef _DEBUG
inline void LogEntities() {
	for (int i = 0; i < Interfaces::EntitySystem->GetHighestEntityIndex(); i++) {
		auto* ent = Interfaces::EntitySystem->GetEntity(i);
		if (ent == nullptr)
			continue;
		//std::cout << ent->SchemaBinding() << '\n';
		const char* className = ent->SchemaBinding()->binaryName;
		if (
			className != nullptr
			//&& strstr(className, "Rune")
			)
			std::cout << className << ' ' << i
			//<< " // " << ent->GetPos2D().x << ' ' << ent->GetPos2D().y
			<< " -> " << ent << '\n';
	}
}
inline void LogModifiers(BaseNpc* npc) {
	std::cout << "modifiers:\n";
	for (const auto& modifier : npc->GetModifierManager()->GetModifierList())
		std::cout << "\t" << modifier->GetName() << '\n';

}
inline void LogInvAndAbilities(BaseNpc* npc = nullptr) {
	if (npc == nullptr)
		npc = assignedHero;

	std::cout << std::dec;
	std::cout << "abilities: " << '\n';
	for (const auto& ability : npc->GetAbilities()) {
		if (ability.name != nullptr)
			std::cout << '\t' << ability.name << " " << H2IDX(ability.handle)
			//<< " CD: " << ability.GetAs<BaseAbility>()->GetCooldown() 
			//<< ' ' << std::dec << ability.GetAs<BaseAbility>()->GetEffectiveCastRange()
			<< ' ' << ability.GetEntity()
			<< '\n';
	}
	std::cout << "inventory: " << '\n';
	for (const auto& item : npc->GetItems()) {
		if (item.name != nullptr)
			std::cout << '\t' << item.name << " " << H2IDX(item.handle)
			<< ' ' << item.GetEntity()
			<< '\n';
	}
}
#endif // _DEBUG
