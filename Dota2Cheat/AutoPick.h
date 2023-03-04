#pragma once
#include "Interfaces.h"
#include <format>
#include "Globals.h"

namespace Hacks {
	class AutoPick {
		bool triedAutoBan = false,
			triedAutoPick = false;
	public:
		const char* autoBanHero;
		const char* autoPickHero;
		// The following two functions accept the name of the hero without npc_dota_hero_ prefix
		// like BanHero("techies")

		void BanHero(const char* name) {
			auto command = std::format("dota_captain_ban_hero npc_dota_hero_{}", name);
			Interfaces::InputService->CmdCommand(command.c_str());
		}
		void PickHero(const char* name) {
			auto command = std::format("dota_select_hero npc_dota_hero_{}", name);
			Interfaces::InputService->CmdCommand(command.c_str());
		}

		void Reset() {
			triedAutoBan = triedAutoPick = false;
		}

		void TryAutoBan() {
			if (ctx.gameStage != Context::GameStage::IN_MATCH || 
				(!autoBanHero && !autoPickHero) 
				|| Globals::GameRules->GetGameState() != DOTA_GAMERULES_STATE_HERO_SELECTION
				)
				return;

			bool isBanPhase = false;
			switch (Globals::GameRules->GetGameMode()) {
			case DOTA_GAMEMODE_ALL_DRAFT:
			case DOTA_GAMEMODE_AP:
			{
				isBanPhase = Globals::GameRules->Member<int>(Netvars::C_DOTAGamerules::m_nAllDraftPhase) == 0;
				break;
			}
			case DOTA_GAMEMODE_TURBO: {
				isBanPhase = Globals::GameRules->Member<int>(Netvars::C_DOTAGamerules::m_BannedHeroes) == 0;
				break;
			}
			};

			if (autoBanHero && isBanPhase && !triedAutoBan) {
				BanHero(autoBanHero);
				triedAutoBan = true;
			}
			else if (autoPickHero && !isBanPhase && !triedAutoPick) {
				PickHero(autoPickHero);
				triedAutoPick = true;
			}
		}
	};
}
namespace Modules {
	Hacks::AutoPick AutoPick{};
}