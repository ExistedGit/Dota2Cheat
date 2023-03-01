// dllmain.cpp : Определяет точку входа для приложения DLL.
#pragma once
#include "pch.h"

#include <cstdio>
#include <iostream>
#include "vtabler.h"
#include "Wrappers.h"
#include "SDK/color.h"
#include "Globals.h"
#include "MatchStateHandling.h"
#include "HookHelper.h"
#include "Input.h"
#include "UIState.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#include "Config.h"
#include "DebugFunctions.h"

#include "Hooks/EntitySystemEvents.h"

#include "Drawing.h"

#include "Lua/LuaModules.h"
#include "Lua/LuaInitialization.h"

#pragma region Static variables

Vector3 Vector3::Zero = Vector3(0, 0, 0);
std::map<std::string, CVarSystem::CVarInfo> CVarSystem::CVar{};
std::vector<std::unique_ptr<IGameEventListener2>> CGameEventManager::EventListeners{};

#pragma endregion

// Simple helper function to load an image into a OpenGL texture with common settings
bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height)
{
	// Load from file
	int image_width = 0;
	int image_height = 0;
	unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
	if (image_data == NULL)
		return false;

	// Create a OpenGL texture identifier
	GLuint image_texture;
	glGenTextures(1, &image_texture);
	glBindTexture(GL_TEXTURE_2D, image_texture);

	// Setup filtering parameters for display
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

	// Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
	stbi_image_free(image_data);

	*out_texture = image_texture;
	*out_width = image_width;
	*out_height = image_height;

	return true;
}


static inline void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

uintptr_t WINAPI HackThread(HMODULE hModule) {
	// Initialize MinHook.
	if (MH_Initialize() != MH_OK)
		FreeLibraryAndExitThread(hModule, 0);

	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);

	ctx.CurProcId = GetCurrentProcessId();
	ctx.CurProcHandle = OpenProcess(PROCESS_ALL_ACCESS, TRUE, ctx.CurProcId);

	// open some common libraries
	ctx.lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::string);
	Lua::InitEnums(ctx.lua);
	Lua::InitClasses(ctx.lua);
	Lua::LoadScriptFiles(ctx.lua);

	ctx.lua.script("print(\"works!\")");
	Interfaces::InitInterfaces();
	Interfaces::LogInterfaces();

	//std::future<void> schemaThread = std::async(std::launch::async, []() mutable {
	//	Schema::SchemaDumpToMap("client.dll", "C_DOTA_BaseNPC_Hero");
	//Schema::SchemaDumpToMap("client.dll", "C_DOTAPlayerController");
	//Schema::SchemaDumpToMap("client.dll", "C_DOTA_UnitInventory");
	//	});

	Lua::InitInterfaces(ctx.lua);
	Interfaces::CVar->DumpConVarsToMap();
#ifdef _DEBUG
	Signatures::InitSignatures(true);
#else
	Signatures::InitSignatures(false);
#endif // _DEBUG


	Globals::InitGlobals();
	Hooks::SetUpByteHooks();
	VMTs::EntitySystem = std::unique_ptr<VMT>(new VMT(Interfaces::EntitySystem));
	VMTs::EntitySystem->HookVM(Hooks::OnAddEntity, 14);
	VMTs::EntitySystem->HookVM(Hooks::OnRemoveEntity, 15);
	VMTs::EntitySystem->ApplyVMT();
	Hooks::SetUpVirtualHooks(true);

//	schemaThread.wait();

	//auto file = Interfaces::FileSystem->OpenFile("scripts/npc/npc_heroes.txt", "r");
	//char buffer[512];
	//while (Interfaces::FileSystem->ReadLine(buffer, 512, file)) {
	//	std::cout << buffer;
	//};


	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
		return 1;

	// GL 3.0 + GLSL 130
	constexpr const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

#ifndef _DEBUG // wouldn't want the window to obscure the screen on a breakpoint
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, 1);
	glfwWindowHint(GLFW_DECORATED, 0);
	glfwWindowHint(GLFW_FLOATING, 1);
	glfwWindowHint(GLFW_RESIZABLE, 0);
	glfwWindowHint(GLFW_MAXIMIZED, 1);
	glfwWindowHint(GLFW_MOUSE_PASSTHROUGH, 1);
#endif // DEBUG

	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only

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
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsClassic();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	//auto vbeFont = io.Fonts->AddFontFromFileTTF(R"(C:\Windows\Fonts\trebuc.ttf)", 80.0f, nullptr, io.Fonts->GetGlyphRangesDefault());
	auto defaultFont = io.Fonts->AddFontDefault();

	char scriptBuf[4096]{};

	bool menuVisible = false;
	bool scriptMenuVisible = false;
	bool circleMenuVisible = false;

	int debugEntIdx = 0;

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::PushFont(defaultFont);
		if (menuVisible) {
			ImGui::Begin("Main", nullptr, ImGuiWindowFlags_::ImGuiWindowFlags_AlwaysAutoResize);

			if (ImGui::Button("Scripting"))
				scriptMenuVisible = !scriptMenuVisible;

			//ImGui::End();


			if (scriptMenuVisible) {

				ImGui::Begin("Scripting");
				ImGui::InputTextMultiline("Lua script", scriptBuf, 4096, ImVec2(300, 500));
				if (ImGui::Button("Execute"))
					ctx.lua.script(scriptBuf);

				ImGui::End();
			}
#ifdef _DEBUG
			ImGui::Begin("Debug functions", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

			if (ImGui::Button("Log Entities")) {
				LogEntities();
			}
			if (ImGui::Button("Log Inventory")) {
				auto selected = ctx.localPlayer->GetSelectedUnits();
				auto ent = (BaseNpc*)Interfaces::EntitySystem->GetEntity(selected[0]);
				LogInvAndAbilities(ent);
			}

			if (ImGui::Button("Log Modifiers")) {
				auto selected = ctx.localPlayer->GetSelectedUnits();
				auto ent = (BaseNpc*)Interfaces::EntitySystem->GetEntity(selected[0]);
				LogModifiers(ent);
			}

			ImGui::InputInt("Entity index", &debugEntIdx, 1, 10);
			if (ImGui::Button("Log entity by index")) {
				auto* ent = Interfaces::EntitySystem->GetEntity(debugEntIdx);
				if (ent == nullptr)
					continue;
				const char* className = ent->SchemaBinding()->binaryName;
				if (
					className
					)
					std::cout << className << ' ' << debugEntIdx
					<< " -> " << ent << '\n';
			}

			ImGui::End();
#endif // _DEBUG



			if (ImGui::Button("Circle drawing"))
				circleMenuVisible = !circleMenuVisible;


			if (ImGui::TreeNode("Visuals")) {
				// https://github.com/SK68-ph/Shadow-Dance-Menu
				ImGui::ListBox(
					"Change weather",
					&Config::WeatherListIdx,
					UIState::WeatherList,
					IM_ARRAYSIZE(UIState::WeatherList),
					4);

				// credits to the screenshot https://yougame.biz/threads/283404/
				// should've figured out it's controlled by a convar like the weather :)
				ImGui::ListBox(
					"River paint",
					&Config::RiverListIdx,
					UIState::RiverList,
					IM_ARRAYSIZE(UIState::RiverList),
					4);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Auto-pickup")) {
				ImGui::Checkbox("Bounty runes", &Config::AutoPickUpRunes);
				ImGui::Checkbox("Aegis", &Config::AutoPickUpAegis);
				ImGui::TreePop();
			}

			//if (ImGui::TreeNode("Visible by Enemy")) {
			//	ImGui::Checkbox("Show HIDDEN/DETECTED text", &Config::VBEShowText);
			//	ImGui::Checkbox("Show a circle under the hero when visible", &Config::VBEShowParticle);
			//	ImGui::TreePop();
			//}
			if (ImGui::TreeNode("Illusion coloring")) {
				ImGui::ColorEdit3("Color", &Config::IllusionColor.x);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Show enemy spells")) {
				ImGui::Checkbox("Point spells", &Config::ShowEnemyPointSpells);
				ImGui::SameLine(); HelpMarker("Sunstrike, Torrent, Light Strike Array");

				if (ImGui::Checkbox("Targeted spells", &Config::ShowEnemyTargetedSpells))
					Modules::TargetedSpellHighlighter.OnDisableTargetedSpells();
				ImGui::SameLine(); HelpMarker("Assassinate, Charge of Darkness");

				if (ImGui::Checkbox("Show Linken Sphere", &Config::ShowLinkenSphere))
					Modules::TargetedSpellHighlighter.OnDisableLinken();
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("AutoWand")) {
				ImGui::Checkbox("Auto-use Faerie Fire and Magic Stick", &Config::AutoWandEnabled);
				ImGui::SliderFloat("Faerie Fire HP Treshold", &Config::AutoHealFaerieFireHPTreshold, 0, 100, "%.1f");
				ImGui::SliderFloat("Magic Stick/Wand/Holy Locket HP Treshold", &Config::AutoHealWandHPTreshold, 0, 100, "%.1f");
				ImGui::SliderInt("Minimum charges", &Config::AutoHealWandMinCharges, 1, 20);

				ImGui::TreePop();
			}
			ImGui::Checkbox("Show all particles", &Config::RenderAllParticles);
			ImGui::SameLine(); HelpMarker("Renders any possible particle, even in FoW");

			ImGui::Checkbox("Show enemy projectile trajectory", &Config::WarnLinearProjectiles);
			ImGui::SameLine(); HelpMarker("Draws a red line for things like Mirana's arrow");


			ImGui::Checkbox("Auto-use Hand of Midas", &Config::AutoMidasEnabled);
			ImGui::Checkbox("Auto-buy Tome of Knowledge", &Config::AutoBuyTome);
			ImGui::SliderFloat("Camera distance", &Config::CameraDistance, 1200, 3000, "%.1f");


			if (ImGui::Button("EXIT", ImVec2(100, 50)))
				glfwSetWindowShouldClose(window, 1);

			ImGui::End();

			if (circleMenuVisible) {
				ImGui::Begin("C I R C L E S", &circleMenuVisible, ImGuiWindowFlags_AlwaysAutoResize);
				ImGui::InputInt("Circle radius", &Config::CircleRadius, 1, 10);
				ImGui::ColorEdit3("Circle RGB", &Config::CircleRGB.x);

				if (ImGui::Button("Draw circle")) {
					Vector3 color = Config::CircleRGB * 255;
					Vector3 radius{ static_cast<float>(Config::CircleRadius), 255, 0 };

					auto particle = Globals::ParticleManager->CreateParticle(
						"particles/ui_mouseactions/selected_ring.vpcf",
						PATTACH_ABSORIGIN_FOLLOW,
						(BaseEntity*)ctx.assignedHero
					).particle
						->SetControlPoint(1, &color)
						->SetControlPoint(2, &radius)
						->SetControlPoint(3, &Vector3::Zero);
				}
				ImGui::End();
			}

		}




		//if (ctx.IsInMatch && Config::VBEShowText)
		//	DrawTextForeground(window, vbeFont, UIState::HeroVisibleToEnemy ? "DETECTED" : "HIDDEN", ImVec2(1920 / 2, 1080 * 3 / 4), 80.0f, Color(200, 200, 200, 255), true);

		if (IsKeyPressed(VK_INSERT)) {
			glfwSetWindowAttrib(window, GLFW_MOUSE_PASSTHROUGH, menuVisible);
			menuVisible = !menuVisible;
		}
#ifdef _DEBUG
		if (ctx.assignedHero) {
			int x = 0, y = 0;
			auto vec = ctx.assignedHero->GetForwardVector(500);
			Signatures::Scripts::WorldToScreen(&vec, &x, &y, nullptr);
			int size = 10;
			DrawRect(window, ImVec2(x - size, y - size), ImVec2(size, size), ImVec4(1, 0, 0, 1));
		}
#endif // _DEBUG

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

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	if (ctx.IsInMatch)
		LeftMatch();
	Modules::TargetedSpellHighlighter.OnDisableTargetedSpells();
	Modules::TargetedSpellHighlighter.OnDisableLinken();

	Schema::Netvars.clear();

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

