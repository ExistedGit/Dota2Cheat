#pragma once
#include "../../SDK/pch.h"
#include <format>

namespace Hacks {
	class AutoPick {
		bool triedAutoBan = false,
			triedAutoPick = false;
	public:
		const char* autoBanHero;
		const char* autoPickHero;
		// The following two functions accept the name of the hero without npc_dota_hero_ prefix
		// like BanHero("techies")

		void BanHero(const char* name);
		void PickHero(const char* name);

		void Reset();

		void TryAutoBan();
	};
}
namespace Modules {
	inline Hacks::AutoPick AutoPick{};
}