#pragma once
#include "../Config.h"
#include "../Modules/Hacks/IllusionColoring.h"
#include "../Modules/Hacks/AegisAutoPickup.h"
#include "../Modules/Hacks/AutoBuyTome.h"
#include "../Modules/Hacks/RiverPaint.h"
#include "../Modules/Hacks/AutoHeal.h"
#include "../Modules/Hacks/ShakerAttackAnimFix.h"
#include "../Modules/Hacks/AbilityESP.h"
#include "../Modules/Hacks/TargetedSpellHighlighter.h"
#include "../Modules/Hacks/LinearProjectileWarner.h"
#include "../Modules/Hacks/AutoPing.h"
#include "../Modules/Hacks/AutoDodge.h"
#include "../Modules/Hacks/AutoMidas.h"
#include "../Modules/Hacks/UIOverhaul.h"
#include "../Modules/Hacks/DotaPlusUnlocker.h"
#include "../Modules/Hacks/SkinChanger.h"

#include "../Modules/Utility/ParticleGC.h"

#include "../Input.h"
#include "../SDK/Entities/CDOTAItemRune.h"

namespace Hooks {
	template<typename T = CBaseEntity>
	std::set<T*> GetEntitiesByFilter(const std::vector<const char*>& filters) {
		std::set<T*> vec{};
		for ( int i = 0; i <= Interfaces::EntitySystem->GetHighestEntityIndex( ); ++i ) {
			auto* ent = Interfaces::EntitySystem->GetEntity(i);
			if (!ent || ent->GetIdentity()->IsDormant())
				continue;
			//std::cout << ent->SchemaBinding() << '\n';
			const char* className = ent->SchemaBinding()->binaryName;
			if (className && TestStringFilters(className, filters))
				vec.insert((T*)ent);
		}
		return vec;
	};

	void EntityIteration();

	void UpdateCameraDistance();
	void UpdateWeather();

	typedef void(__fastcall* RunFrameFn)(void* thisptr);
	inline RunFrameFn oRunFrame{};
	void hkRunFrame(void* thisptr);
}