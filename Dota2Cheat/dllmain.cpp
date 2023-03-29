#pragma once
#define STB_IMAGE_IMPLEMENTATION
#define SOL_ALL_SAFETIES_ON
#include <cstdio>
#include <iostream>
#include "HookHelper.h"
#include "Input.h"
#include "UIState.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#include "Config.h"
#include "DebugFunctions.h"

#include "Hooks/EntitySystemEvents.h"

#include "Utils/Drawing.h"

#include "Lua/LuaModules.h"
#include "Lua/LuaInitialization.h"

#include "MatchStateHandling.h"
#include "Hooks/InvalidateUEF.h"
#include "UI/Pages/MainMenu.h"
#include "UI/Pages/AutoPickSelectionGrid.h"

#pragma region Static variables

Vector Vector::Zero = Vector(0, 0, 0);
std::vector<std::unique_ptr<IGameEventListener2>> CGameEventManager::EventListeners{};

#pragma endregion

static inline void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}


uintptr_t WINAPI HackThread(HMODULE hModule) {
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

		std::ifstream fin(ctx.cheatFolderPath + "\\config\\base.json");
		if (fin.is_open()) {
			Config::cfg.LoadConfig(fin);
			fin.close();
		}
	}
	ctx.CurProcId = GetCurrentProcessId();
	ctx.CurProcHandle = OpenProcess(PROCESS_ALL_ACCESS, TRUE, ctx.CurProcId);

	ctx.lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::string, sol::lib::math);

	ctx.lua.script("print(\"works!\")");
	Interfaces::FindInterfaces();

	Interfaces::CVar->DumpConVarsToMap();
#ifdef _DEBUG
	Signatures::FindSignatures(true);
#else
	Signatures::FindSignatures(false);
#endif // _DEBUG

	GameSystems::FindGameSystems();
	Hooks::SetUpByteHooks();
	Hooks::SetUpVirtualHooks(true);
	Hooks::DisableHooks();

	Lua::InitEnums(ctx.lua);
	Lua::InitClasses(ctx.lua);
	Lua::InitInterfaces(ctx.lua);
	Lua::InitFunctions(ctx.lua);
	Lua::SetGlobals(ctx.lua);
	Lua::LoadScriptFiles(ctx.lua);

	//	Panorama::CSource2UITexture* texture{};
	//	Signatures::LoadUITexture(nullptr, (void**)&texture, "panorama\\images\\hero_badges\\hero_badge_rank_1_png.vtex");

	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
		return 1;

	// GL 3.0 + GLSL 130
	constexpr const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, 1);
#ifndef _DEBUG // wouldn't want the window to obscure the screen on a breakpoint
	glfwWindowHint(GLFW_DECORATED, 0);
	glfwWindowHint(GLFW_FLOATING, 1);
	glfwWindowHint(GLFW_RESIZABLE, 0);
	glfwWindowHint(GLFW_MAXIMIZED, 1);
	glfwWindowHint(GLFW_MOUSE_PASSTHROUGH, 1);
#endif // DEBUG
	auto* monitor = glfwGetPrimaryMonitor();
	if (!monitor)
		return 0;

	auto videoMode = glfwGetVideoMode(monitor);

	GLFWwindow* window = glfwCreateWindow(videoMode->width, videoMode->height, "Dota2Cheat", NULL, NULL);
	if (window == NULL)
		return 1;
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Setup Dear ImGui style
	ImGui::StyleColorsClassic();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	//auto vbeFont = io.Fonts->AddFontFromFileTTF(R"(C:\Windows\Fonts\trebuc.ttf)", 80.0f, nullptr, io.Fonts->GetGlyphRangesDefault());
	auto msTrebuchet = io.Fonts->AddFontFromFileTTF(R"(C:\Windows\Fonts\trebuc.ttf)", 80.0f, nullptr, io.Fonts->GetGlyphRangesDefault());
	auto defaultFont = io.Fonts->AddFontDefault();
	bool menuVisible = false;

	Modules::AbilityESP.textFont = msTrebuchet;
	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::PushFont(defaultFont);

		if (Interfaces::Engine->IsInGame()) {
			texManager.ExecuteLoadCycle();
			Modules::AbilityESP.DrawESP();
		}

		if (menuVisible)
			Pages::MainMenu::Draw(window);

#ifdef _DEBUG
		// Pages::AutoPickHeroGrid::Draw(window);
#endif // _DEBUG
		if (IsKeyPressed(VK_INSERT)) {
			glfwSetWindowAttrib(window, GLFW_MOUSE_PASSTHROUGH, menuVisible);
			menuVisible = !menuVisible;
		}

		ImGui::PopFont();

		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);

		CheckMatchState(); // checking every frame
	}

	std::ofstream fout(ctx.cheatFolderPath + "\\config\\base.json");
	Config::cfg.SaveConfig(fout);
	fout.close();

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	if (ctx.gameStage != Context::GameStage::NONE)
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
		//imgui ver
		HANDLE thread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, 0);
		if (thread)
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

