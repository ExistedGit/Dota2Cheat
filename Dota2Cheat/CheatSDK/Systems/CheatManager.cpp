#include "CheatManager.h"

#include <fstream>

#include "../Config.h"
#include "TextureManager.h"
#include "EventManager.h"

#include "../Hooking.h"
#include "../../Modules/UI/BarAugmenter.h"
#include "../../Modules/Hacks/IllusionColoring.h"
#include "../../Modules/Utility/CVarSpoofer.h"
#include "../../Modules/Utility/CVarSpoofer.h"
#include "../../UI/Pages/AutoPickSelectionGrid.h"
#include "../Globals/VMTDB.h"

void CCheatManager::LoadGameSpecific() {
	// Allows VPK mods
	// IDK what became of that piece of code
	//if (auto gi = Memory::Scan("74 ? 84 C9 75 ? 83 BF", "client.dll"))
	//	Memory::Patch(gi, { 0xEB });

	LogFI("Loading VM indices from {}", cheatFolderPath + "\\vmt.json");
	VMDB::LoadFromFile(cheatFolderPath + "\\vmt.json");
	VMDB::ParseMap(VMI::vmTables);

	LogFI("Loading signatures from {}", cheatFolderPath + "\\signatures.json");
	SignatureDB::LoadSignaturesFromFile(cheatFolderPath + "\\signatures.json");
	Signatures::FindSignatures();

	// Disables gameoverlayrenderer64's WINAPI hook checks
	// e. g. they track the use of VirtualProtect(Ex) with PAGE_EXECUTE_READWRITE specifically
	if (auto enableWinAPIHooks = Memory::Scan("76 0A 45 84 FF", "gameoverlayrenderer64.dll"))
		enableWinAPIHooks
		.Offset(0xC)
		.GetAbsoluteAddress(2, 7)
		.Set(false);

	Interfaces::FindInterfaces();

	Interfaces::CVar->DumpConVarsToMap();
#ifdef _DEBUG
	Interfaces::CVar->UnlockHiddenConVars();
#endif
	//Modules::CVarSpoofer.SpoofVar("r_farz");
	//Modules::CVarSpoofer.SpoofVars(
	//	"dota_camera_distance",
	//	"r_farz",
	//	"fog_enable",
	//	"dota_river_type",
	//	"cl_weather",
	//	"dota_hud_chat_enable_all_emoticons"
	//);

	Interfaces::CVar->CVars["dota_hud_chat_enable_all_emoticons"].m_pVar->value.boolean = Config::Changer::UnlockEmoticons;

	GameSystems::FindGameSystems();

#if defined(_DEBUG) && !defined(_TESTING)
	LogD("ItemSchema: ", Signatures::GetItemSchema());
#endif

	Modules::BarAugmenter.Init();

	Hooks::InstallHooks();

	EventManager.InstallListeners();

	EntityList.AddListener(Modules::IllusionESP);

	CBaseEntity::OnColorChanged = Interfaces::NetworkMessages->FindCallback("OnColorChanged");
}

void CCheatManager::LoadFiles() {
	if (auto fin = std::ifstream(cheatFolderPath + "\\config\\base.json")) {
		Config::cfg.LoadConfig(fin);
		fin.close();

		LogI("Loaded config from ", cheatFolderPath, "\\config\\base.json\n");
	}
	locale.Init(Config::Locale);
}

void CCheatManager::Initialize(HMODULE hModule) {

	this->hModule = hModule;
	AllocConsole();
	freopen_s(&consoleStream, "CONOUT$", "w", stdout);

	// don't you dare touch this line
	Log(LP_NONE, "works!");

	LogI("Initializing...");
	FindCheatFolder();

	Config::cfg.SetupVars();
	LoadGameSpecific();

	LoadFiles();
	LogI("Initialization complete!");
}

void CCheatManager::SaveConfig() {
	if (auto fout = std::ofstream(cheatFolderPath + "\\config\\base.json")) {
		Config::cfg.SaveConfig(fout);
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

	Hooks::RemoveHooks();
	Memory::RevertPatches();
	MH_Uninitialize();
	if (DrawData.Dx.Window)
		SetWindowLongPtrA(DrawData.Dx.Window, GWLP_WNDPROC, (LONG_PTR)DrawData.Dx.oWndProc);

	Modules::CVarSpoofer.RestoreVars();

	if (consoleStream) fclose(consoleStream);
	FreeConsole();
	FreeLibrary(hModule);
}
