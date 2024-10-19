#pragma once
#include "../../CheatSDK/include.h"
#include <functional>

namespace Modules {
	namespace BCP {
		using CastHandler = std::function<bool(CDOTABaseAbility* spell, CDOTABaseNPC* caster, CDOTABaseNPC* target)>;
		namespace CastHandlers {
			inline bool GSSoulChain(CDOTABaseAbility* spell, CDOTABaseNPC* caster, CDOTABaseNPC* target) {
				const static auto latchRadius = spell->GetLevelSpecialValueFor("chain_latch_radius");
				return EntityList.ContainsTypes([target](const auto& wrap) {
					auto hero = wrap.As<CDOTABaseNPC>();
					return hero != target &&
						hero->IsSameTeam(target)
						&& IsWithinRadius(hero->GetPos(), target->GetPos(), latchRadius);
					}, EntityType::Hero);
			};
		};

		// Probably should also check for enemy presence in the process of casting
		class M_BadCastPrevention {
			const std::set<std::string_view> pointAbilityNames = {
				"faceless_void_chronosphere",
				"enigma_black_hole"
			};
		public:
			bool AreEnemyUnitsInArea(const Vector& center, int radius);
			bool AreEnemyHeroesInArea(const Vector& center, int radius);
			
			// Ability name -> handler
			const std::map<std::string_view, CastHandler> castHandlers = {
				{
					"grimstroke_soul_chain", CastHandlers::GSSoulChain
				}
			};

			// Checks whether the ability is cast at an area without enemy heroes/units
			bool IsBadCast(dotaunitorder_t orderType, UINT32 targetIndex, Vector* position, UINT32 abilityIndex, CBaseEntity* issuer);
		};
	}
	inline BCP::M_BadCastPrevention BadCastPrevention{};
}