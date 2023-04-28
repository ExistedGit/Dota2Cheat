#include "MatchStateHandling.h"

#define GetGameSystem(global) GameSystems::##global = *GameSystems::## global ##Ptr; LogF(LP_DATA, "{}: {}", #global, (void*)GameSystems::global);

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

// Now that the iteration is based on collections, the cheat does not retain the entity lists upon reinjection
void CacheAllEntities() {
	for (int i = 0; i <= Interfaces::EntitySystem->GetHighestEntityIndex(); i++) {
		auto ent = Interfaces::EntitySystem->GetEntity(i);
		if (!IsValidReadPtr(ent) ||
			!IsValidReadPtr(ent->SchemaBinding()->binaryName))
			continue;

		SortEntToCollections(ent);
	}
}
void OnUpdatedAssignedHero()
{
	ctx.lua["localHero"] = ctx.assignedHero;
	Lua::CallModuleFunc("OnUpdatedAssignedHero");

	for (auto& modifier : ctx.assignedHero->GetModifierManager()->GetModifierList())
		Hooks::CacheIfItemModifier(modifier); // for registering important items on reinjection

	Modules::ShakerAttackAnimFix.SubscribeEntity(ctx.assignedHero);
}

void UpdateAssignedHero() {
	if (!ctx.localPlayer)
		return;

	auto heroHandle = ctx.localPlayer->GetAssignedHeroHandle();
	if (ctx.assignedHeroHandle != heroHandle) {
		auto assignedHero = Interfaces::EntitySystem->GetEntity<CDOTABaseNPC_Hero>(H2IDX(ctx.localPlayer->GetAssignedHeroHandle()));

		ctx.assignedHeroHandle = heroHandle;
		ctx.assignedHero = assignedHero;

		LogF(LP_INFO, "Changed hero: \n\tHandle: {:X}\n\tEntity: {}", heroHandle, (void*)assignedHero);
		if (assignedHero)
			OnUpdatedAssignedHero();
	}
}

void EnteredPreGame() {

	//	Modules::AutoPick.autoBanHero = "sniper";
	//	Modules::AutoPick.autoPickHero = "arc_warden";
	
	ctx.localPlayer = Signatures::GetPlayer(-1);
	if (!ctx.localPlayer)
		return;

	GetGameSystem(GameRules);
	GetGameSystem(ProjectileManager);
	GetGameSystem(PlayerResource);
	GetGameSystem(ParticleManager);
	GetGameSystem(GameEventManager);

	ctx.gameStage = Context::GameStage::PRE_GAME;

	if (!oFireEventClientSide) {
		auto vmt = VMT(GameSystems::GameEventManager);
		void* FireEventClientSide = vmt.GetVM(8);
		HOOKFUNC(FireEventClientSide);
	}

	Log(LP_INFO, "GAME STAGE: PRE-GAME");
}

void EnteredInGame() {
	DOTA_GameState gameState = GameSystems::GameRules->GetGameState();
	if (gameState != DOTA_GAMERULES_STATE_PRE_GAME &&
		gameState != DOTA_GAMERULES_STATE_GAME_IN_PROGRESS)
		return;

	//if (GameSystems::GameRules->GetGameMode() == DOTA_GAMEMODE_CUSTOM)
	//	return;

	UpdateAssignedHero();

	if (!ctx.assignedHero)
		return;

	//Config::AutoPingTarget = ctx.assignedHero;
	//FillPlayerList();

	Log(LP_INFO, "GAME STAGE: INGAME");
	LogF(LP_DATA, "Local Player: {}\n\tSTEAM ID: {}", (void*)ctx.localPlayer, ctx.localPlayer->GetSteamID());
	if (ctx.assignedHero->GetUnitName())
		LogF(LP_DATA, "Assigned Hero: {} {}", (void*)ctx.assignedHero, ctx.assignedHero->GetUnitName());

	Interfaces::CVar->SetConvars();

	//auto roshanListener = new RoshanListener();
	//roshanListener->gameStartTime = GameSystems::GameRules->GetGameTime();
	//auto runel = new RunePickupListener();
	//GameSystems::GameEventManager->AddListener(roshanListener, "dota_roshan_kill");
	//GameSystems::GameEventManager->AddListener(runel, "dota_rune_pickup");

	Lua::SetGlobals(ctx.lua);

	CacheAllEntities();

	Modules::AbilityESP.SubscribeHeroes();
	Modules::KillIndicator.Init();
	//Modules::UIOverhaul.Init();

	Lua::CallModuleFunc("OnJoinedMatch");

	ctx.gameStage = Context::GameStage::IN_GAME;
}

void LeftMatch() {
	ctx.gameStage = Context::GameStage::NONE;

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

	GameSystems::GameEventManager = nullptr;

	ctx.localPlayer = nullptr;
	ctx.assignedHero = nullptr;
	ctx.assignedHeroHandle = 0xFFFFFFFF;

	Lua::SetGlobals(ctx.lua);
	texManager.QueueTextureUnload();

	Log(LP_INFO, "GAME STAGE: NONE");
}

void CheckMatchState() {
	if (Interfaces::Engine->IsInGame()) {
		//Modules::AutoPick.TryAutoBan();
		if (ctx.gameStage == Context::GameStage::NONE)
			EnteredPreGame();
		else if (ctx.gameStage == Context::GameStage::PRE_GAME)
			EnteredInGame();
		else if (ctx.gameStage == Context::GameStage::IN_GAME)
			UpdateAssignedHero();
	}
	else if (ctx.gameStage != Context::GameStage::NONE)
		LeftMatch();
}
