#include "CheatManager.h"

#include <fstream>
#include <MinHook.h>

#include "../Config.h"
#include "EventManager.h"
#include "TextureManager.h"

#include "../../Modules/Hacks/IllusionColoring.h"
#include "../../Modules/Hacks/TargetedSpellHighlighter.h"
#include "../../Modules/UI/AbilityESP/AbilityESP.h"
#include "../../Modules/UI/BarAugmenter.h"
#include "../../Modules/Utility/CVarSpoofer.h"
#include "../../UI/Pages/AutoPickSelectionGrid.h"
#include "../Globals/VMTDB.h"
#include "../Hooking.h"
#include "../Tables.h"

void CCheatManager::LoadVMI() {
	auto vmtPath = GetResource("vmt.json");

	LogFI("Loading VM indices from {}", vmtPath);
	VMDB::LoadFromFile(vmtPath);
	VMDB::ParseMap(VMI::bindings);
}

void CCheatManager::LoadGameSpecific() {
	auto signaturePath = GetResource("signatures.json");

	LogFI("Loading signatures from {}", signaturePath);
	SignatureDB::LoadSignaturesFromFile(signaturePath);
	Signatures::FindSignatures();

	LoadVMI();

	// Disables gameoverlayrenderer64's WINAPI hook checks
	// e. g. they track the use of VirtualProtect(Ex) with PAGE_EXECUTE_READWRITE specifically
	if (auto enableWinAPIHooks = Memory::Scan("76 0A 45 84 FF", "gameoverlayrenderer64.dll"))
		enableWinAPIHooks
		.Offset(0xC)
		.GetAbsoluteAddress(2, 7)
		.Set(false);

	LogI("[ INTERFACES ]");

	tables::PrettyPrint({
		{ "CSource2Client", CSource2Client::Get()},
		{ "CGameEntitySystem", CEntSys::Get() },
		{ "CGCClient", CGCClient::Get() },
		{ "CCVar", CCVar::Get() },
		{ "CResourceSystem", CResourceSystem::Get() },
		{ "CBaseFileSystem", CFileSys::Get() },
		{ "CGameUI", CGameUI::Get() },
		{ "CSoundOpSystem", CSoundOpSys::Get() },
		{ "CInputService", CInputService::Get() },

		{ "CPanoramaUIEngine", CPanoramaUIEngine::Get() },
		{ "CUIEngineSource2", CUIEngine::Get() },

		{ "INetworkClientService", INetworkClientService::Get()},
		{ "CNetworkMessages", CNetworkMessages::Get() },
		{ "CFlattenedSerializers", CFlattenedSerializers::Get() },

		{ "ISteamClient", ISteamClient::Get() },
		{ "ISteamGameCoordinator", ISteamGC::Get() },

		// Unused but pertinent interfaces
		{ "CEngineClient", Memory::GetInterfaceBySubstr("engine2.dll", "Source2EngineToClient0") },
		{ "CSchemaSystem", Memory::GetInterfaceBySubstr("schemasystem.dll", "SchemaSystem") },
		});

	CCVar::Get()->DumpConVarsToMap();
#ifdef _DEBUG
	CCVar::Get()->UnlockHiddenConVars();
#endif
	//Modules::CVarSpoofer.SpoofVar("r_farz");

#ifndef _DEBUG 
	Modules::CVarSpoofer.SpoofVars(
		"dota_camera_distance",
		"r_farz",
		"fog_enable",
		"dota_river_type",
		"cl_weather",
		"dota_hud_chat_enable_all_emoticons"
	);
#endif

	CCVar::Get()->CVars["dota_hud_chat_enable_all_emoticons"].m_pVar->value.boolean = Config::Changer::UnlockEmoticons;

	GameSystems::FindGameSystems();

//#if defined(_DEBUG) && !defined(_TESTING)
//	LogD("ItemSchema: ", Signatures::GetItemSchema());
//#endif

	Hooks::InstallHooks();
	Hooks::InstallAuxiliaryHooks();

	EventManager.InstallListeners();

	CBaseEntity::OnColorChanged = CNetworkMessages::Get()->FindCallback("OnColorChanged");
}

void CCheatManager::LoadFiles() {
	auto cfgPath = GetResource("config\\base.json");

	if (auto fin = std::ifstream(cfgPath)) {
		Config::cfg.Deserialize(fin);
		fin.close();

		LogFI("Loaded config from {}", cfgPath);
	}
	else {
		LogI("No config found, proceeding with defaults");
	}
	locale.Init(Config::Locale);
}

void CCheatManager::Initialize(HMODULE hModule) {
	if (MH_Initialize() != MH_OK)
		FreeLibrary(hModule);

	this->hModule = hModule;
	AllocConsole();
	freopen_s(&consoleStream, "CONOUT$", "w", stdout);

	// don't you dare touch this line
	Log(LP_NONE, "works!");

	LogI("Initializing...");
	FindCheatFolder();

	Config::cfg.SetupVars();
	LoadFiles();
	LoadGameSpecific();

	LogI("Initialization complete!");
}

void CCheatManager::SaveConfig() {
	auto cfgPath = GetResource("config\\base.json");
	if (auto fout = std::ofstream(cfgPath)) {
		Config::cfg.Serialize(fout);
		fout.close();
	};
}

void CCheatManager::Unload() {
	SaveConfig();

	if (ctx.gameStage != GameStage::NONE)
		MatchStateManager.LeftMatch();

	Modules::TargetedSpellHighlighter.OnDisableTargetedSpells();
	Modules::TargetedSpellHighlighter.OnDisableLinken();
	EventManager.ClearListeners();

	Hooks::RemoveAuxiliaryHooks();
	Memory::RevertPatches();
	MH_Uninitialize();
	if (DrawData.Dx.Window)
		SetWindowLongPtrA(DrawData.Dx.Window, GWLP_WNDPROC, (LONG_PTR)DrawData.Dx.oWndProc);

	//Modules::CVarSpoofer.RestoreVars();

	if (consoleStream) fclose(consoleStream);
	FreeConsole();
	FreeLibrary(hModule);
}
