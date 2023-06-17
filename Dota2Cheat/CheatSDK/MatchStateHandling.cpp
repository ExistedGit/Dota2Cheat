#include "MatchStateHandling.h"

void FillPlayerList() {
	auto vec = GameSystems::PlayerResource->GetVecPlayerTeamData();
	std::cout << "<PLAYERS>\n";
	for (auto& data : vec) {
		auto slot = data.GetPlayerSlot();

		auto player = Interfaces::EntitySystem->GetEntity<CDOTAPlayerController>(
			H2IDX(
				GameSystems::PlayerResource->PlayerSlotToHandle(slot)
			)
			);
		if (!player)
			continue;

		auto hero = player->GetAssignedHero();
		std::cout << "Player " << std::dec << slot << ": " << player;
		if (hero &&
			hero->GetUnitName()) {
			std::cout << "\n\t" << hero->GetUnitName() << " " << hero;
			ctx.heroes.insert(hero);
		}

		std::cout << '\n';
		//players.push_back(player);
	}
}


void CMatchStateManager::EnteredPreGame() {

	//	Modules::AutoPick.autoBanHero = "sniper";
	//	Modules::AutoPick.autoPickHero = "arc_warden";

#define DereferenceReallocatingSystem(global) GameSystems::##global = *GameSystems::## global ##Ptr; LogF(LP_DATA, "{}: {}", #global, (void*)GameSystems::global);

	DereferenceReallocatingSystem(PlayerResource);

	if(!GameSystems::PlayerResource)
		return;

 	ctx.localPlayer = GameSystems::PlayerResource->PlayerSlotToHandle(Interfaces::Engine->GetLocalPlayerSlot());
	if (!ctx.localPlayer)
		return;

	GameSystems::ParticleManager = GameSystems::ParticleManagerSystem->GetParticleManager();
	DereferenceReallocatingSystem(GameEventManager);
	DereferenceReallocatingSystem(ProjectileManager);
	DereferenceReallocatingSystem(GlobalVars);

	// Panorama's HUD root
	for (auto& node : Interfaces::UIEngine->GetPanelList<4096>()) {
		auto uiPanel = node.uiPanel;
		if (!uiPanel->GetId())
			continue;
		std::string_view id = uiPanel->GetId();
		if (id != "DotaHud")
			continue;

		GameSystems::DotaHud = (Panorama::DotaHud*)uiPanel;
		break;
	}

	ctx.gameStage = GameStage::PRE_GAME;

	Log(LP_INFO, "GAME STAGE: PRE-GAME");
}

void CMatchStateManager::EnteredInGame() {
	//if (GameSystems::GameRules->GetGameMode() == DOTA_GAMEMODE_CUSTOM)
	//	return;

	//Config::AutoPingTarget = ctx.localHero;
	//FillPlayerList();

	Log(LP_INFO, "GAME STAGE: INGAME");
	LogF(LP_DATA, "Local Player: {}\n\tSTEAM ID: {}", (void*)ctx.localPlayer, ctx.localPlayer->GetSteamID());

	// Interfaces::CVar->CVars["sv_cheats"].m_pVar->value.boolean = true;
	Interfaces::CVar->CVars["r_farz"].m_pVar->value.flt = 10000.0f;
	Interfaces::CVar->CVars["fog_enable"].m_pVar->value.boolean = false;

	GameSystems::InitMinimapRenderer();
	// Modules::UIOverhaul.Init();

	Lua::SetGlobals(ctx.lua);
	Lua::CallModuleFunc("OnJoinedMatch");

	ctx.gameStage = GameStage::IN_GAME;
}

void CMatchStateManager::LeftMatch() {
	ctx.gameStage = GameStage::NONE;

	Lua::CallModuleFunc("OnLeftMatch");

	GameSystems::ParticleManager->OnExitMatch();

	Modules::TargetedSpellHighlighter.Reset();
	Modules::AutoPick.Reset();
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
	GameSystems::DotaHud = nullptr;
	GameSystems::GameEventManager = nullptr;
	ClearHeroData();

	ctx.localPlayer = nullptr;
	ctx.localHero = nullptr;

	Lua::SetGlobals(ctx.lua);
	texManager.QueueTextureUnload();

	Log(LP_INFO, "GAME STAGE: NONE");
}

void CMatchStateManager::CheckForOngoingGame() {
	if (Interfaces::Engine->IsInGame()) {
		MatchStateManager.CacheAllEntities();

		if (GameSystems::GameRules) {
			MatchStateManager.EnteredPreGame();

			if (ctx.localPlayer)
				MatchStateManager.OnUpdatedAssignedHero();

			if (GameSystems::GameRules->GetGameState() == DOTA_GAMERULES_STATE_PRE_GAME ||
				GameSystems::GameRules->GetGameState() == DOTA_GAMERULES_STATE_GAME_IN_PROGRESS) {

				MatchStateManager.EnteredInGame();
				if (ctx.localHero)
					for (auto& modifier : ctx.localHero->GetModifierManager()->GetModifierList()) {
						Hooks::CacheIfItemModifier(modifier); // for registering items on reinjection
						Hooks::CacheModifier(modifier);
					}
			}
		}
	}
}


// D2C's entity system is based on collections
// Without such caching it doesn't retain the entity lists upon reinjection

void CMatchStateManager::CacheAllEntities() {
	for (int i = 0; i <= Interfaces::EntitySystem->GetHighestEntityIndex(); i++) {
		auto ent = Interfaces::EntitySystem->GetEntity(i);
		if (!IsValidReadPtr(ent) ||
			!IsValidReadPtr(ent->SchemaBinding()->binaryName))
			continue;

		SortEntToCollections(ent);

		if (ent->SchemaBinding()->binaryName && !strcmp(ent->SchemaBinding()->binaryName, "C_DOTAGamerulesProxy"))
			GameSystems::GameRules = ent->Member<CDOTAGameRules*>(Netvars::C_DOTAGamerulesProxy::m_pGameRules);
	}
}

void CMatchStateManager::OnUpdatedAssignedHero() {
	CDOTABaseNPC_Hero* assignedHero = ctx.localPlayer->GetAssignedHeroHandle();

	ctx.localHero = assignedHero;

	LogF(LP_INFO, "Changed hero: \n\tEntity: {}\n\tName: {}", (void*)assignedHero, assignedHero->GetUnitName());

	ctx.lua["localHero"] = ctx.localHero;

	Modules::AbilityESP.SubscribeHeroes();
	Modules::KillIndicator.Init();
	Modules::ShakerAttackAnimFix.SubscribeEntity(ctx.localHero);

	Lua::CallModuleFunc("OnUpdatedAssignedHero");
}

void CMatchStateManager::OnStateChanged(DOTA_GameState newState) {
	switch (newState) {
	case DOTA_GAMERULES_STATE_PRE_GAME:
	case DOTA_GAMERULES_STATE_GAME_IN_PROGRESS:
		EnteredInGame();
		break;
	default:
		if (ctx.gameStage == GameStage::NONE)
			EnteredPreGame();
		break;
	}
}
