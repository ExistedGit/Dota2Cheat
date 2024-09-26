#include "MatchStateHandling.h"

void CMatchStateManager::EnteredPreGame() {
#define DereferenceReallocatingSystem(global) if(!GameSystems::##global) { \
												GameSystems::##global = *GameSystems::## global ##Ptr;\
												LogFD("{}: {}", #global, (void*)GameSystems::global);\
											  }

	if (!GameSystems::PlayerResource)
		DereferenceReallocatingSystem(PlayerResource);

	if (!GameSystems::PlayerResource)
		return;

	ctx.localPlayer = Signatures::GetPlayer(-1);
	
	if (!ctx.localPlayer)
		return;

	#ifdef _DEBUG
		LogFI("Player indices:\n\t+1: {}\n\tGetPlayer: {}\n\tPlayerResource: {}",
			Interfaces::NetworkClientService->GetIGameClient()->GetLocalPlayerID() + 1,
			Signatures::GetPlayer(-1)->GetIndex(),
			H2IDX(GameSystems::PlayerResource->PlayerIDToHandle(Interfaces::NetworkClientService->GetIGameClient()->GetLocalPlayerID()))
			);
	#endif

	DereferenceReallocatingSystem(GameEventManager);
	DereferenceReallocatingSystem(ProjectileManager);
	//DereferenceReallocatingSystem(RenderGameSystem);

	GameSystems::ParticleManager = GameSystems::ParticleManagerSystem->GetParticleManager();
	LogFD("ParticleManager: {} / CreateParticle: {}", (void*)GameSystems::ParticleManager, (void*)GameSystems::ParticleManager->GetVFunc(VMI::CDOTAParticleManager::CreateParticle));

	ctx.gameStage = GameStage::PRE_GAME;

	LogI("GAME STAGE: PRE-GAME");
}

void CMatchStateManager::EnteredInGame() {
	//if (GameSystems::GameRules->GetGameMode() == DOTA_GAMEMODE_CUSTOM)
	//	return;

	LogI("GAME STAGE: INGAME");
	LogFD("Local Player: {}\n\tSTEAM ID: {}", (void*)ctx.localPlayer, ctx.localPlayer->GetSteamID());

	// Interfaces::CVar->CVars["sv_cheats"].m_pVar->value.boolean = true;
	Interfaces::CVar->CVars["r_farz"].m_pVar->value.flt = 10000.0f;
	Interfaces::CVar->CVars["fog_enable"].m_pVar->value.boolean = false;

	Panorama::FindPanels();
	Modules::UIOverhaul.QueueUpdateNetworthPanel();

	//for (auto& data : GameSystems::PlayerResource->GetVecPlayerTeamData()) {
	//	data.SelectedHeroBadgeXP() = 72050;
	//}

	GameSystems::InitMinimapRenderer();
	if (Config::Changer::TreeModelIdx != 0)
		Modules::TreeChanger.QueueModelUpdate(Config::Changer::TreeModelIdx - 1);

	ctx.gameStage = GameStage::IN_GAME;
}

void CMatchStateManager::LeftMatch() {
	ctx.gameStage = GameStage::NONE;


	GameSystems::ParticleManager->OnExitMatch();

	Modules::TargetedSpellHighlighter.Reset();
	Modules::AutoPick.Reset();
	Modules::TreeChanger.Reset();
	Modules::ParticleGC.Reset();
	Modules::AbilityESP.Reset();
	Modules::UIOverhaul.Reset();
	Modules::TPTracker.Reset();
	Modules::KillIndicator.Reset();

	GameSystems::PlayerResource = nullptr;
	GameSystems::GameRules = nullptr;
	GameSystems::ParticleManager = nullptr;
	GameSystems::ProjectileManager = nullptr;
	GameSystems::MinimapRenderer = nullptr;
	GameSystems::GameEventManager = nullptr;

	HeroData.clear();

	Panorama::DotaHud = nullptr;
	Panorama::ErrorMessages = nullptr;

	ctx.localPlayer = nullptr;
	ctx.localHero = nullptr;

	LogI("GAME STAGE: NONE");
}

void CMatchStateManager::CheckForOngoingGame() {
	if (!Interfaces::NetworkClientService->GetIGameClient()->IsInGame())
		return;

	MatchStateManager.CacheAllEntities();

	if (!GameSystems::GameRules)
		return;

	MatchStateManager.EnteredPreGame();

	if (ctx.localPlayer)
		MatchStateManager.OnUpdatedAssignedHero();

	if (GameSystems::GameRules->GetGameState() == DOTA_GAMERULES_STATE_PRE_GAME ||
		GameSystems::GameRules->GetGameState() == DOTA_GAMERULES_STATE_GAME_IN_PROGRESS) {

		EnteredInGame();
		if (ctx.localHero)
			for (auto& modifier : ctx.localHero->GetModifierManager()->GetModifierList()) {
				Hooks::CacheIfItemModifier(modifier); // for registering items on reinjection
				Hooks::CacheModifier(modifier);
			}
	}
}


void CMatchStateManager::CacheAllEntities() {
	for (int i = 0; i <= Interfaces::EntitySystem->GetHighestEntityIndex(); i++) {
		auto ent = Interfaces::EntitySystem->GetEntity(i);
		if (!IsValidReadPtr(ent) ||
			!IsValidReadPtr(ent->SchemaBinding()->binaryName))
			continue;

		EntityList.OnEntityCreated(ent);
	}
}

void CMatchStateManager::OnUpdatedAssignedHero() {
	CDOTABaseNPC_Hero* assignedHero = ctx.localPlayer->GetAssignedHeroHandle();

	ctx.localHero = assignedHero;

	LogFI("Changed hero: \n\tEntity: {}\n\tName: {}", (void*)assignedHero, assignedHero->GetUnitName());

	Modules::AbilityESP.SubscribeHeroes();
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
