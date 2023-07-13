#pragma once
#include "../../pch.h"
#include "../../CheatSDK/include.h"
#include <format>

namespace Modules {
inline 
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
	} AutoPick;
}