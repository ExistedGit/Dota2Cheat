#pragma once
#define STB_IMAGE_IMPLEMENTATION

// Will use local copy of signatures.json instead of the one from GitHub.
// Use if you need to update them and the "cloud" version isn't updated yet/will not be updated
#ifdef _DEBUG
#define D2C_USE_LOCAL_SIGNATURES
#endif

#include <cstdio>
#include <iostream>
#include "CheatSDK/Hooking.h"

#include "DebugFunctions.h"

#include "Utils/Drawing.h"

#include "CheatSDK/include.h"
#include "CheatSDK/MatchStateHandling.h"
#include "CheatSDK/Lua/LuaInitialization.h"

#include "Hooks/InvalidateUEF.h"
#include "Modules/UI/Indicators/SpeedIndicator.h"
#include "Modules/UI/Indicators/KillIndicator.h"
#include "Modules/UI/Indicators/HookIndicator.h"
#include "UI/Pages/MainMenu.h"
#include "UI/Pages/AutoPickSelectionGrid.h"
#include "Modules/Hacks/LastHitMarker.h"
#include "Modules/UI/BarAugmenter.h"
#include "SDK/Interfaces/GC/CEconWearable.h"

GLFWwindow* window_menu{};

#pragma region Static variables

Vector Vector::Zero = Vector(0, 0, 0);

#pragma endregion

static inline void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

uintptr_t WINAPI HackThread(HMODULE hModule) {
	constexpr bool useChangerCode = false;
	// Initialize MinHook.
	if (MH_Initialize() != MH_OK)
		FreeLibraryAndExitThread(hModule, 0);

	Hooks::InvalidateUEF::Create();
	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);
	Config::cfg.SetupVars();

	{
		char buf[256];
		SHGetSpecialFolderPathA(0, buf, CSIDL_PROFILE, false);
		ctx.cheatFolderPath = buf;
		ctx.cheatFolderPath += "\\Documents\\Dota2Cheat";
		{
			std::ifstream fin(ctx.cheatFolderPath + "\\config\\base.json");
			if (fin.is_open()) {
				Config::cfg.LoadConfig(fin);
				fin.close();
				std::cout << "Loaded config from " << ctx.cheatFolderPath + "\\config\\base.json\n";
			}
		}

		for (auto& file : std::filesystem::directory_iterator(ctx.cheatFolderPath + "\\assets\\misc")) {
			auto path = file.path();
			auto fileName = path.filename().string();
			texManager.QueueForLoading(path.string(), fileName.substr(0, fileName.size() - 4));
		}
		if (useChangerCode) {
			std::ifstream fin(ctx.cheatFolderPath + "\\config\\inventory.json");
			if (fin.is_open()) {
				Config::cfg.LoadEquippedItems(fin);
				fin.close();
				std::cout << "Loaded inventory from " << ctx.cheatFolderPath + "\\config\\inventory.json\n";
			}
		}

		Modules::SkinChanger.DeleteSOCacheFiles();
	}

	// Allows VPK mods
	if (auto gi = Memory::Scan("74 ? 84 C9 75 ? 83 BF", "client.dll"))
		Memory::Patch(gi, { 0xEB });


	ctx.lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::string, sol::lib::math);

	ctx.lua.script("print(\"works!\")");

	Interfaces::FindInterfaces();

	auto iconLoadThread = std::async(std::launch::async, []() {
		Pages::AutoPickHeroGrid::InitList();
	return true;
		});

	Interfaces::CVar->DumpConVarsToMap();

#ifdef D2C_USE_LOCAL_SIGNATURES
	SignatureDB::LoadSignaturesFromFile(ctx.cheatFolderPath + "\\signatures.json");
#else
	SignatureDB::LoadSignaturesFromNetwork("https://raw.githubusercontent.com/ExistedGit/Dota2Cheat/main/signatures.json");
#endif

	Signatures::FindSignatures();
	GameSystems::FindGameSystems();

	if (useChangerCode) {
		std::ifstream fin(ctx.cheatFolderPath + "\\assets\\itemdefs.txt");
		if (fin.is_open())
		{
			Modules::SkinChanger.ParseItemDefs(fin);
			fin.close();
		}
	}

	Log(LP_DATA, "ItemSchema: ", Signatures::GetItemSchema());

	Hooks::SetUpByteHooks();
	Hooks::SetUpVirtualHooks(true);

	Lua::InitEnums(ctx.lua);
	Lua::InitClasses(ctx.lua);
	Lua::InitInterfaces(ctx.lua);
	Lua::InitFunctions(ctx.lua);
	Lua::SetGlobals(ctx.lua);
	Lua::LoadScriptFiles(ctx.lua);

	std::cout << "Loading finished, initializing UI\n";

	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
		return 1;

	// GL 3.0 + GLSL 130
	constexpr const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, 1);

	// wouldn't want the window to obscure the screen on a breakpoint
#if !defined(_DEBUG) || defined(_TESTING)
	glfwWindowHint(GLFW_FLOATING, 1);
#endif // DEBUG
	glfwWindowHint(GLFW_DECORATED, 0);
	glfwWindowHint(GLFW_RESIZABLE, 0);
	glfwWindowHint(GLFW_MAXIMIZED, 1);
	glfwWindowHint(GLFW_MOUSE_PASSTHROUGH, 0);
	auto* monitor = glfwGetPrimaryMonitor();
	if (!monitor)
		return 0;

	const auto videoMode = glfwGetVideoMode(monitor);

	window_menu = glfwCreateWindow(videoMode->width, videoMode->height, "Dota2Cheat", NULL, NULL);
	if (window_menu == NULL)
		return 1;

	glfwMakeContextCurrent(window_menu);
	glfwSwapInterval(1); // Enable vsync

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Setup Dear ImGui style
	ImGui::StyleColorsClassic();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window_menu, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	Log(LP_INFO, "Loading fonts...");
	auto defaultFont = io.Fonts->AddFontDefault();
	{
		ImFontConfig fontCfg{};
		fontCfg.FontDataOwnedByAtlas = false;
		for (int i = 2; i < 30; i += 2) {
			DrawData.Fonts["MSTrebuchet"][i] = io.Fonts->AddFontFromFileTTF(R"(C:\Windows\Fonts\trebuc.ttf)", i, nullptr, io.Fonts->GetGlyphRangesDefault());
			DrawData.Fonts["Monofonto"][i] = io.Fonts->AddFontFromMemoryTTF((void*)Fonts::Monofonto, IM_ARRAYSIZE(Fonts::Monofonto), i, &fontCfg, io.Fonts->GetGlyphRangesDefault());
		}
	}
	bool menuVisible = true;
	std::cout << "Icon loading result: " << iconLoadThread.get() << "\n";
	int itemDefId = 6996;
	// Main loop
	while (!glfwWindowShouldClose(window_menu))
	{
		glfwPollEvents();

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		texManager.ExecuteLoadCycle();

#ifdef _DEBUG
		// Pages::AutoPickHeroGrid::Draw();
#endif // _DEBUG
		ImGui::PushFont(DrawData.Fonts["MSTrebuchet"][24]);
		if (
			GameSystems::GameUI->GetUIState() == DOTA_GAME_UI_DOTA_INGAME
			&& ctx.gameStage == GameStage::IN_GAME
			&& ctx.localHero
			) {
			Modules::AbilityESP.DrawESP();
			// Modules::UIOverhaul.DrawBars();
			Modules::TPTracker.DrawMapTeleports();
			Modules::LastHitMarker.Draw();
			Modules::BlinkRevealer.Draw();
			Modules::ParticleMaphack.Draw();
			Modules::BarAugmenter.Draw();

			Modules::SpeedIndicator.Draw();
			Modules::KillIndicator.Draw();
			//Modules::HookIndicator.Draw();

			//const auto ScreenSize = glfwGetVideoMode(glfwGetPrimaryMonitor());
			//auto ActualMinimapSize = static_cast<float>(GameSystems::MinimapRenderer->GetMinimapSize().y - 28);
			//auto MinimapPosMin = ImVec2(16, static_cast<float>(ScreenSize->height - ActualMinimapSize - 12));

			//ImGui::GetForegroundDrawList()->AddRectFilled(MinimapPosMin, MinimapPosMin + ImVec2{ ActualMinimapSize, ActualMinimapSize }, ImColor{ 255,0,0 });

		}
		ImGui::PopFont();

		ImGui::PushFont(defaultFont);

		if (menuVisible)
			Pages::MainMenu::Draw();

		if (IsKeyPressed(VK_INSERT)) {
			glfwSetWindowAttrib(window_menu, GLFW_MOUSE_PASSTHROUGH, menuVisible);
			menuVisible = !menuVisible;
		}

#ifdef _DEBUG

		ImGui::InputInt("ItemDef ID", &itemDefId);
		if (ImGui::Button("Create item"))
			Modules::SkinChanger.QueueAddItem(itemDefId);
#endif // _DEBUG

		ImGui::PopFont();

		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window_menu, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window_menu);

		CheckMatchState(); // checking every frame
	}
	{

		std::ofstream fout(ctx.cheatFolderPath + "\\config\\base.json");
		Config::cfg.SaveConfig(fout);
		fout.close();
	}
	{
		std::ofstream fout(ctx.cheatFolderPath + "\\config\\inventory.json");
		Config::cfg.SaveEquippedItems(fout);
		fout.close();
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window_menu);
	glfwTerminate();

	if (ctx.gameStage != GameStage::NONE)
		LeftMatch();
	Modules::TargetedSpellHighlighter.OnDisableTargetedSpells();
	Modules::TargetedSpellHighlighter.OnDisableLinken();

	Hooks::InvalidateUEF::Remove();
	MH_Uninitialize();
	if (f) fclose(f);
	FreeConsole();
	FreeLibraryAndExitThread(hModule, 0);
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH: {
		if (HANDLE thread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, 0); thread)
			CloseHandle(thread);
		break;
	}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}