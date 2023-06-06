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

void CMatchStateManager::EnteredPreGame() {

	//	Modules::AutoPick.autoBanHero = "sniper";
	//	Modules::AutoPick.autoPickHero = "arc_warden";

	ctx.localPlayer = Signatures::GetPlayer(-1);
	if (!ctx.localPlayer)
		return;

#define DereferenceReallocatingSystem(global) GameSystems::##global = *GameSystems::## global ##Ptr; LogF(LP_DATA, "{}: {}", #global, (void*)GameSystems::global);

	DereferenceReallocatingSystem(PlayerResource);
	DereferenceReallocatingSystem(ParticleManager);
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

	// I HATE SIGNATURES
	// I HATE SIGNATURES
	// we've got you surrounded!
	// come scan for a 40-byte signature!
	if (!Hooks::oFireEventClientSide) {
		auto vmt = VMT(GameSystems::GameEventManager);
		void* FireEventClientSide = vmt.GetVM(8);
		HookFunc(FireEventClientSide, &Hooks::hkFireEventClientSide, &Hooks::oFireEventClientSide, "FireEventClientSide");
	}

	Log(LP_INFO, "GAME STAGE: PRE-GAME");
}

void CMatchStateManager::EnteredInGame() {
	//if (GameSystems::GameRules->GetGameMode() == DOTA_GAMEMODE_CUSTOM)
	//	return;

	//Config::AutoPingTarget = ctx.localHero;
	//FillPlayerList();

	Log(LP_INFO, "GAME STAGE: INGAME");
	LogF(LP_DATA, "Local Player: {}\n\tSTEAM ID: {}", (void*)ctx.localPlayer, ctx.localPlayer->GetSteamID());

	Interfaces::CVar->CVars["sv_cheats"].m_pVar->value.boolean = true;
	Interfaces::CVar->CVars["r_farz"].m_pVar->value.flt = 10000.0f;
	Interfaces::CVar->CVars["fog_enable"].m_pVar->value.boolean = false;

	GameSystems::InitMinimapRenderer();

	Lua::SetGlobals(ctx.lua);

	// CacheAllEntities();

	//Modules::UIOverhaul.Init();

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