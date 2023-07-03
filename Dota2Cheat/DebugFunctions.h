#pragma once
#include "SDK/Entities/CDOTABaseNPC.h"
#include "SDK/Globals/Interfaces.h"
#include "SDK/Base/VMT.h"

#ifdef _DEBUG
inline void LogEntities() {
	std::stringstream buf;
	for (int idx = 0; idx <= Interfaces::EntitySystem->GetHighestEntityIndex(); ++idx) {
		if ( CBaseEntity* entity = Interfaces::EntitySystem->GetEntity( idx ); ( entity && IsValidReadPtr( entity ) ) ) {
			const auto schema_bind = entity->SchemaBinding( );
			if ( entity->GetIdentity( )->GetName( ) && schema_bind && schema_bind->binaryName ) {
				buf << (void*)entity << ", " << entity->GetIdentity( )->GetName( ) << ", " << schema_bind->binaryName << std::endl;
			}
		}
	}

	const auto str = buf.str( );
	std::cout << str;
}
inline void LogModifiers(CDOTABaseNPC* npc) {
	std::cout << "modifiers:\n";
	for (const auto& modifier : npc->GetModifierManager()->GetModifierList())
		std::cout << "\t" << modifier->GetName() << ' ' << modifier << '\n';
}
inline void LogInvAndAbilities(CDOTABaseNPC* npc = nullptr) {
	if (npc == nullptr)
		npc = ctx.localHero;

	std::cout << std::dec;
	std::cout << "abilities: " << '\n';
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

#endif // _DEBUG
