#include "MatchStateHandling.h"
#include "Tables.h"

#include "../Hooks/ModifierEvents.h"

#include "Data/UIData.h"
#include "../Modules/Hacks/AutoBuyTome.h"
#include "../Modules/UI/AbilityESP/AbilityESP.h"
#include "../Modules/UI/UIOverhaul.h"
#include "../Modules/UI/TPTracker.h"
#include "../Modules/UI/Indicators/KillIndicator.h"
#include "../Modules/Hacks/ShakerAttackAnimFix.h"
#include "../Modules/Hacks/TreeChanger.h"

//#include "../Modules/Hacks/AutoPick.h"
//#include "../Modules/Hacks/AutoPing.h"

void CMatchStateManager::EnteredPreGame() {
	if (!CPlayerResource::Get())
		return;

	ctx.localPlayer = Signatures::GetPlayer(-1);

	if (!ctx.localPlayer)
		return;

	LogI("GAME STAGE: PRE-GAME");

#ifdef _DEBUG
	LogFI("Player indices:\n\t+1: {}\n\tGetPlayer: {}\n\tPlayerResource: {}",
		INetworkClientService::Get()->GetIGameClient()->GetLocalPlayerID() + 1,
		Signatures::GetPlayer(-1)->GetIndex(),
		CPlayerResource::Get()->PlayerIDToHandle(INetworkClientService::Get()->GetIGameClient()->GetLocalPlayerID()).Index()
	);
#endif

	LogI("[ REALLOCATING GAME SYSTEMS ]");

	tables::PrettyPrint({
		{ "CDOTAParticleManager", CParticleMgr::Get() },
		{ "C_DOTA_PlayerResource", CPlayerResource::Get() },
		{ "CRenderGameSystem", CRenderSys::Get() },
		{ "C_DOTA_ProjectileManager", CProjectileMgr::Get() },
		{ "CGameEventManager", CEventMgr::Get() },
		});

	ctx.gameStage = GameStage::PRE_GAME;
}

void CMatchStateManager::EnteredInGame() {
	//if (CGameRules::Get()->GetGameMode() == DOTA_GAMEMODE_CUSTOM)
	//	return;

	LogI("GAME STAGE: INGAME");
	LogFD("Local Player: {}\n\tSTEAM ID: {}", (void*)ctx.localPlayer, ctx.localPlayer->GetSteamID());

	// CCVar::Get()->CVars["sv_cheats"].m_pVar->value.boolean = true;
	CCVar::Get()->CVars["r_farz"].m_pVar->value.flt = 10000.0f;
	CCVar::Get()->CVars["fog_enable"].m_pVar->value.boolean = false;

	Panorama::FindPanels();
	Modules::UIOverhaul.QueueUpdateNetworthPanel();

	//for (auto& data : CPlayerResource::Get()->GetVecPlayerTeamData()) {
	//	data.SelectedHeroBadgeXP() = 72050;
	//}

	GameSystems::InitMinimapRenderer();
	if (Config::Changer::TreeModelIdx != 0)
		Modules::TreeChanger.QueueModelUpdate(Config::Changer::TreeModelIdx - 1);

	ctx.gameStage = GameStage::IN_GAME;
}

void CMatchStateManager::LeftMatch() {
	ctx.gameStage = GameStage::NONE;

	CParticleMgr::Get()->OnExitMatch();

	Modules::TargetedSpellHighlighter.Reset();
	Modules::TreeChanger.Reset();
	Modules::ParticleGC.Reset();
	Modules::UIOverhaul.Reset();
	Modules::TPTracker.Reset();
	Modules::KillIndicator.Reset();

	CGameRules::Set(nullptr);
	GameSystems::MinimapRenderer = nullptr;

	HeroData.clear();

	Panorama::DotaHud = nullptr;
	Panorama::ErrorMessages = nullptr;

	ctx.localPlayer = nullptr;
	ctx.localHero = nullptr;

	LogI("GAME STAGE: NONE");
}

void CMatchStateManager::CheckForOngoingGame() {
	if (!INetworkClientService::Get()->GetIGameClient()->IsInGame())
		return;

	MatchStateManager.CacheAllEntities();

	if (!CGameRules::Get())
		return;

	MatchStateManager.EnteredPreGame();

	if (ctx.localPlayer)
		MatchStateManager.OnUpdatedAssignedHero();

	if (CGameRules::Get()->GetGameState() == DOTA_GAMERULES_STATE_PRE_GAME ||
		CGameRules::Get()->GetGameState() == DOTA_GAMERULES_STATE_GAME_IN_PROGRESS) {

		EnteredInGame();
		if (ctx.localHero)
			for (auto& modifier : ctx.localHero->GetModifierManager()->GetModifierList()) {
				Hooks::CacheIfItemModifier(modifier); // for registering items on reinjection
				Hooks::CacheModifier(modifier);
			}
	}
}


void CMatchStateManager::CacheAllEntities() {
	for (int i = 0; i <= CEntSys::Get()->GetHighestEntityIndex(); i++) {
		auto ent = CEntSys::Get()->GetEntity(i);
		if (!ent || !ent->SchemaBinding() ||!ent->SchemaBinding()->binaryName)
			continue;

		EntityList.OnEntityCreated(ent);
	}
}

void CMatchStateManager::OnUpdatedAssignedHero() {
	CDOTABaseNPC_Hero* assignedHero = ctx.localPlayer->GetAssignedHeroHandle();

	ctx.localHero = assignedHero;

	LogFI("Changed hero: \n\tEntity: {}\n\tName: {}", (void*)assignedHero, assignedHero->GetUnitName());

	Modules::KillIndicator.Init();
	Modules::ShakerAttackAnimFix.SubscribeEntity(ctx.localHero);
}

void CMatchStateManager::OnStateChanged(DOTA_GameState newState) {
	switch (newState) {
	case DOTA_GAMERULES_STATE_PRE_GAME:
	case DOTA_GAMERULES_STATE_GAME_IN_PROGRESS:
		// if (ctx.gameStage != GameStage::IN_GAME)
		EnteredInGame();
		break;
	default:
		if (ctx.gameStage == GameStage::NONE)
			EnteredPreGame();
		break;
	}
}
