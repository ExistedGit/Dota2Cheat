#include "AutoPick.h"

void Modules::AutoPick::BanHero(const char* name) {
	auto command = std::format("dota_captain_ban_hero npc_dota_hero_{}", name);
	Interfaces::InputService->CmdCommand(command.c_str());
}

void Modules::AutoPick::PickHero(const char* name) {
	auto command = std::format("dota_select_hero npc_dota_hero_{}", name);
	Interfaces::InputService->CmdCommand(command.c_str());
}

void Modules::AutoPick::Reset() {
	triedAutoBan = triedAutoPick = false;
}

void Modules::AutoPick::TryAutoBan() {
	if (ctx.gameStage != GameStage::PRE_GAME ||
		(!autoBanHero && !autoPickHero)
		|| CGameRules::Get()->GetGameState() != DOTA_GAMERULES_STATE_HERO_SELECTION
		)
		return;

	bool isBanPhase = false;
	switch (CGameRules::Get()->GetGameMode()) {
	case DOTA_GAMEMODE_ALL_DRAFT:
	case DOTA_GAMEMODE_AP:
	{
		isBanPhase = CGameRules::Get()->Member<int>(Netvars::C_DOTAGamerules::m_nAllDraftPhase) == 0;
		break;
	}
	case DOTA_GAMEMODE_TURBO: {
		isBanPhase = CGameRules::Get()->Member<int>(Netvars::C_DOTAGamerules::m_BannedHeroes) == 0;
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
