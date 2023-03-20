#include "MatchStateHandling.h"
#include "HookHelper.h"
#include "Lua/LuaModules.h"
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
		else if (!strcmp(className, "C_DOTA_Item_Rune")) {
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

	ctx.localPlayer = Interfaces::EntitySystem->GetEntity<CDOTAPlayerController>(
		H2IDX(
			GameSystems::PlayerResource->PlayerSlotToHandle(Interfaces::Engine->GetLocalPlayerSlot())
		)
		);
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
	if (!ctx.assignedHero) {
		//ctx.localPlayer = Interfaces::EntitySystem->GetEntity<CDOTAPlayerController>(Interfaces::Engine->GetLocalPlayerSlot() + 2);
		//ctx.assignedHero = Interfaces::EntitySystem->GetEntity<CDOTABaseNPC_Hero>(H2IDX(ctx.localPlayer->GetAssignedHeroHandle()));
		//if (!ctx.assignedHero)
		return;
	}

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

	Lua::SetGlobals(ctx.lua);

	ReinjectEntIteration();

	Modules::ShakerAttackAnimFix.SubscribeEntity(ctx.assignedHero);
	Modules::AutoBuyTome.Init();
	Modules::AbilityESP.SubscribeHeroes();

	Lua::CallModuleFunc("Init");

	Hooks::EnableHooks();
	ctx.gameStage = Context::GameStage::IN_GAME;
	std::cout << "ENTERED GAME\n";
}

void LeftMatch() {
	ctx.gameStage = Context::GameStage::NONE;

	Lua::CallModuleFunc("Reset");

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

	for (auto& listener : CGameEventManager::EventListeners)
		GameSystems::GameEventManager->RemoveListener(listener.get());
	CGameEventManager::EventListeners.clear();
	GameSystems::GameEventManager = nullptr;

	ctx.localPlayer = nullptr;
	ctx.assignedHero = nullptr;
	
	Lua::SetGlobals(ctx.lua);
	Hooks::DisableHooks();
	texManager.QueueTextureUnload();

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
