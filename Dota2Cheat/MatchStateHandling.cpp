#include "MatchStateHandling.h"


void FillPlayerList() {
	auto vec = GameSystems::PlayerResource->GetVecPlayerTeamData();
	std::cout << "<PLAYERS>\n";
	for (int i = 0; i < vec.m_Size; i++) {
		//Sleep(800);
		auto idx = vec[i].GetPlayerSlot() + 1;
		if (idx < 1)
			continue;

		auto player = (CDOTAPlayerController*)Interfaces::EntitySystem->GetEntity(idx);
		if (player == nullptr)
			continue;

		auto hero = (CDOTABaseNPC*)player->GetAssignedHero();
		//std::cout << idx << " " << player << ' ' << player->GetIdentity() << '\n';

		std::cout << "Player " << std::dec << idx << ": " << player;
		if (hero &&
			hero->GetUnitName()) {
			std::cout << "\n\t" << hero->GetUnitName() << " " << hero;
			ctx.heroes.insert((CDOTABaseNPC_Hero*)hero);
		}
		std::cout << '\n';

		//players.push_back(player);
	}
}

// Now that the iteration is based on collections, the cheat does not retain the entity lists upon reinjection
void ReinjectEntIteration() {
	for (int i = 0; i <= Interfaces::EntitySystem->GetHighestEntityIndex(); i++) {
		auto ent = Interfaces::EntitySystem->GetEntity(i);
		if (!ent)
			continue;
		auto className = ent->SchemaBinding()->binaryName;
		if (!className)
			continue;
		if (strstr(className, "Item_Physical")) {
			ctx.physicalItems.insert(ent);
		}
		else if (!strcmp(className, "CDOTAItemRune")) {
			ctx.runes.insert((CDOTAItemRune*)ent);
		}
		else if (strstr(className, "Unit_Hero")) {
			ctx.heroes.insert(reinterpret_cast<CDOTABaseNPC_Hero*>(ent));
		}
		ctx.entities.insert(ent);
	}
}

void EnteredMatch() {
	GetGameSystem(GameRules);
	GetGameSystem(ProjectileManager);
	GetGameSystem(PlayerResource);
	GetGameSystem(ParticleManager);
	GetGameSystem(GameEventManager);

	//	Modules::AutoPick.autoBanHero = "sniper";
	//	Modules::AutoPick.autoPickHero = "arc_warden";

	ctx.localPlayer = (CDOTAPlayerController*)Interfaces::EntitySystem->GetEntity(Interfaces::Engine->GetLocalPlayerSlot() + 1);
	if (!ctx.localPlayer)
		return;

	ctx.gameStage = Context::GameStage::IN_MATCH;
	std::cout << "ENTERED MATCH\n";
}

void EnteredInGame() {
	DOTA_GameState gameState = GameSystems::GameRules->GetGameState();
	if (gameState != DOTA_GAMERULES_STATE_PRE_GAME &&
		gameState != DOTA_GAMERULES_STATE_GAME_IN_PROGRESS)
		return;

	ctx.assignedHero = Interfaces::EntitySystem->GetEntity<CDOTABaseNPC_Hero>(H2IDX(ctx.localPlayer->GetAssignedHeroHandle()));
	if (!ctx.assignedHero)
		return;

	//Config::AutoPingTarget = ctx.assignedHero;
	for (auto& modifier : ctx.assignedHero->GetModifierManager()->GetModifierList())
		Hooks::CacheIfItemModifier(modifier); // for registering important items on reinjection


	//FillPlayerList();


	std::cout << "Local Player: " << ctx.localPlayer
		<< "\n\t" << std::dec << "STEAM ID: " << ctx.localPlayer->GetSteamID()
		<< '\n';
	std::cout << "Assigned Hero: " << ctx.assignedHero << " " << ctx.assignedHero->GetUnitName() << '\n';

	Interfaces::CVar->SetConvars();

	auto roshanListener = new RoshanListener();
	roshanListener->gameStartTime = GameSystems::GameRules->GetGameTime();
	auto hurtListener = new EntityHurtListener();
	GameSystems::GameEventManager->AddListener(roshanListener, "dota_roshan_kill");
	GameSystems::GameEventManager->AddListener(hurtListener, "entity_hurt");

	GameSystems::LogGameSystems();
	Lua::InitGlobals(ctx.lua);

	ctx.lua["assignedHero"] = ctx.assignedHero;
	ctx.lua["localPlayer"] = ctx.localPlayer;

	ReinjectEntIteration();

	Modules::ShakerAttackAnimFix.SubscribeEntity(ctx.assignedHero);
	Modules::AutoBuyTome.Init();
	Modules::AbilityESP.SubscribeHeroes();

	ctx.gameStage = Context::GameStage::IN_GAME;
	std::cout << "ENTERED GAME\n";
}

void LeftMatch() {
	ctx.gameStage = Context::GameStage::NONE;

	GameSystems::ParticleManager->OnExitMatch();

	Modules::AutoBuyTome.Reset();
	Modules::TargetedSpellHighlighter.Reset();
	Modules::AutoPick.Reset();
	Modules::ParticleGC.Reset();
	Modules::AbilityESP.Reset();

	GameSystems::PlayerResource = nullptr;
	GameSystems::GameRules = nullptr;
	GameSystems::ParticleManager = nullptr;
	GameSystems::ProjectileManager = nullptr;

	Lua::ResetGlobals(ctx.lua);

	for (auto& listener : CGameEventManager::EventListeners)
		GameSystems::GameEventManager->RemoveListener(listener.get());
	CGameEventManager::EventListeners.clear();
	GameSystems::GameEventManager = nullptr;


	ctx.localPlayer = nullptr;
	ctx.assignedHero = nullptr;
	ctx.lua["assignedHero"] = nullptr;
	ctx.lua["localPlayer"] = nullptr;

	std::cout << "LEFT MATCH\n";
}

void CheckMatchState() {
	if (Interfaces::Engine->IsInGame()) {
		//Modules::AutoPick.TryAutoBan();
		if (ctx.gameStage == Context::GameStage::NONE)
			EnteredMatch();
		else if (ctx.gameStage == Context::GameStage::IN_MATCH)
			EnteredInGame();
	}
	else if (ctx.gameStage != Context::GameStage::NONE)
		LeftMatch();
}
