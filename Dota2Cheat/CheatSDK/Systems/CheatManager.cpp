#include "CheatManager.h"

#include <fstream>

#include "../../Modules/Utility/CVarSpoofer.h"

#include "../Config.h"
#include "TextureManager.h"
#include "EventManager.h"

#include "../Hooking.h"
#include "../../Modules/UI/BarAugmenter.h"
#include "../../UI/Pages/AutoPickSelectionGrid.h"

void CCheatManager::LoadGameSpecific() {
	Modules::BarAugmenter.Init();

	// Allows VPK mods
	if (auto gi = Memory::Scan("74 ? 84 C9 75 ? 83 BF", "client.dll"))
		Memory::Patch(gi, { 0xEB });

	// Disables gameoverlayrenderer64's WINAPI hook checks
	if (auto enableVACHooks = Memory::Scan("75 04 84 DB", "gameoverlayrenderer64.dll"))
		enableVACHooks
		.Offset(6)
		.GetAbsoluteAddress(2, 7)
		.Set(false);

	Interfaces::FindInterfaces();

	Interfaces::CVar->DumpConVarsToMap();
#ifdef _DEBUG
	Interfaces::CVar->UnlockHiddenConVars();
#endif

	// Log(LP_WARNING, "dota_camera_distance: ", (void*)Interfaces::CVar->CVars["dota_camera_distance"].m_pVar);
	Modules::CVarSpoofer.SpoofVars(
		"dota_camera_distance",
		"r_farz",
		"fog_enable",
		"dota_river_type",
		"cl_weather",
		"dota_hud_chat_enable_all_emoticons"
	);
	Interfaces::CVar->CVars["dota_hud_chat_enable_all_emoticons"].m_pVar->value.boolean = Config::Changer::UnlockEmoticons;

	SignatureDB::LoadSignaturesFromFile(cheatFolderPath + "\\signatures.json");

	Signatures::FindSignatures();
	GameSystems::FindGameSystems();

#if defined(_DEBUG) && !defined(_TESTING)
	Log(LP_DATA, "ItemSchema: ", Signatures::GetItemSchema());
#endif

	Hooks::InstallHooks();

	EventManager.InstallListeners();

	EntityList.AddListener(Modules::IllusionESP);

	// It's supposed to be a CUtlSymbolTable, but we don't yet have the technology...
	for (const auto& data : Interfaces::NetworkMessages->GetNetvarCallbacks())
		if (IsValidReadPtr(data.m_szCallbackName) && std::string_view(data.m_szCallbackName) == "OnColorChanged") {
			CBaseEntity::OnColorChanged = data.m_CallbackFn;
			LogF(LP_DATA, "{}::{}: {}", data.m_szClassName, data.m_szCallbackName, (void*)data.m_CallbackFn);
		}
		else if (IsValidReadPtr(data.m_szCallbackName) && std::string_view(data.m_szCallbackName) == "OnWearablesChanged")
			LogF(LP_DATA, "{}::{}: {}", data.m_szClassName, data.m_szCallbackName, (void*)data.m_CallbackFn);
}

void CCheatManager::LoadFiles() {
	if (auto fin = std::ifstream(cheatFolderPath + "\\config\\base.json")) {
		Config::cfg.LoadConfig(fin);
		fin.close();

		Log(LP_INFO, "Loaded config from ", cheatFolderPath, "\\config\\base.json\n");
	}

	for (auto& file : std::filesystem::directory_iterator(cheatFolderPath + "\\assets\\misc")) {
		auto path = file.path();
		auto fileName = path.filename().string();
		texManager.QueueForLoading(path.string(), fileName.substr(0, fileName.size() - 4));
	}
}

void CCheatManager::Initialize(HMODULE hModule) {

	this->hModule = hModule;
	AllocConsole();
	freopen_s(&consoleStream, "CONOUT$", "w", stdout);

	// Note to everyone: do not remove
	// this is a historical exclamation by myself when something finally works
	Log(LP_NONE, "works!");

	FindCheatFolder();

	Config::cfg.SetupVars();
	LoadGameSpecific();

	LoadFiles();
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
	FreeLibraryAndExitThread(hModule, 0);
}
