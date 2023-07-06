#include "CheatManager.h"

#include <fstream>

#include "../../Modules/Utility/CVarSpoofer.h"

#include "../Config.h"
#include "TextureManager.h"
#include "EventManager.h"
#include <ShlObj.h>

#include "../Lua/LuaInitialization.h"
#include "../Hooking.h"

// Loads and executes scripts from the cheat's folder in C:\Users\%USER%\Documents\Dota2Cheat\scripts

void CCheatManager::LoadScriptFiles() {
	lua.create_named_table("Modules");
	auto scriptsPath = cheatFolderPath + R"(\scripts)";
	if (!std::filesystem::exists(scriptsPath))
		std::cout << scriptsPath << " not found! No scripts for you\n";

	for (auto& file : directory_iterator(scriptsPath)) {
		auto path = file.path();
		if (path.string().substr(path.string().size() - 3) == "lua")
			lua.load_file(path.string())();
	}
}

void CCheatManager::LoadLua() {
	d2c.lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::string, sol::lib::math);

	Lua::InitEnums(d2c.lua);
	Lua::InitClasses(d2c.lua);
	Lua::InitInterfaces(d2c.lua);
	Lua::InitFunctions(d2c.lua);
	Lua::SetGlobals(d2c.lua);
}

void CCheatManager::LoadGameSpecific() {

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

	// It's supposed to be a CUtlSymbolTable, but we don't yet have the technology...
	for (auto data : Interfaces::NetworkMessages->GetNetvarCallbacks())
		if (IsValidReadPtr(data.m_szCallbackName) && std::string_view(data.m_szCallbackName) == "OnColorChanged") {
			CBaseEntity::OnColorChanged = data.m_CallbackFn;
			LogF(LP_DATA, "{}::{}: {}", data.m_szClassName, data.m_szCallbackName, (void*)data.m_CallbackFn);
		}
		else if (IsValidReadPtr(data.m_szCallbackName) && std::string_view(data.m_szCallbackName) == "OnWearablesChanged")
			LogF(LP_DATA, "{}::{}: {}", data.m_szClassName, data.m_szCallbackName, (void*)data.m_CallbackFn);
		//else if (IsValidReadPtr(data.m_szCallbackName) && std::string_view(data.m_szCallbackName) == "skeletonMeshGroupMaskChanged")
		//	LogF(LP_DATA, "{}::{}: {}", data.m_szClassName, data.m_szCallbackName, (void*)data.m_CallbackFn);
}

void CCheatManager::LoadFiles() {
	if (auto fin = std::ifstream(cheatFolderPath + "\\config\\base.json")) {
		Config::cfg.LoadConfig(fin);
		fin.close();
		std::cout << "Loaded config from " << cheatFolderPath + "\\config\\base.json\n";
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

	Config::cfg.SetupVars();
	LoadGameSpecific();
	LoadLua();
	LoadScriptFiles();
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

	// Cleanup
	//ImGui_ImplDX11_Shutdown();
	//ImGui_ImplWin32_Shutdown();
	//ImGui::DestroyContext();

	if (ctx.gameStage != GameStage::NONE)
		MatchStateManager.LeftMatch();

	Modules::TargetedSpellHighlighter.OnDisableTargetedSpells();
	Modules::TargetedSpellHighlighter.OnDisableLinken();
	EventManager.ClearListeners();

	Hooks::RemoveHooks();
	MH_Uninitialize();

	if (DrawData.Dx.Window)
		SetWindowLongPtrA(DrawData.Dx.Window, GWLP_WNDPROC, (LONG_PTR)DrawData.Dx.oWndProc);

	if (consoleStream) fclose(consoleStream);
	FreeConsole();
	FreeLibraryAndExitThread(hModule, 0);
}
