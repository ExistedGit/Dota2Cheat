﻿// dllmain.cpp : Определяет точку входа для приложения DLL.
#pragma once
#include "pch.h"
#include <cstdio>
#include <iostream>
#include "vtabler.h"
#include "Wrappers.h"
#include "SDK/color.h"
#include "Globals.h"
#include "MatchStateHandling.h"
#include "Hooks.h"
#include "Input.h"
#include "UIState.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "GL/glew.h"
#include <GLFW/glfw3.h> // Will drag system OpenGL headers
#include <sstream>
#include "Config.h"
#include "DebugFunctions.h"



#pragma region Global variables

bool IsInMatch = false;
Vector3 Vector3::Zero = Vector3(0, 0, 0);
std::map<std::string, CVarSystem::CVarInfo> CVarSystem::CVar{};
std::vector<std::unique_ptr<IGameEventListener2>> CGameEventManager::EventListeners{};

DotaPlayer* localPlayer;
BaseNpc* assignedHero;
std::vector<DotaPlayer*> players{};
std::vector<BaseEntity*> physicalItems{};

HANDLE CurProcHandle;
int CurProcId;
#pragma endregion


//void gotoxy(int x, int y)
//{
//	COORD coord;
//	coord.X = x;
//	coord.Y = y;
//	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
//}


static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

//std::vector<Color> rainbow = {
//	Color(255,0,0),//red
//	Color(255,255,0),
//	Color(0,255,0),//green
//	Color(0,255,255),
//	Color(0,0,255),//blue
//	Color(255,0,255)
//};
//int rainbowIndex = 0;

std::vector<BaseNpc*> enemyHeroes{};

//credits to SMBB from UnknownCheats
//https://www.unknowncheats.me/forum/direct3d/244074-imgui-d3d11-text-drawing.html
float DrawTextForeground(GLFWwindow* wnd, ImFont* pFont, const std::string& text, const ImVec2& pos, float size, Color color, bool center)
{
	float r = color.RGBA[0];
	float g = color.RGBA[1];
	float b = color.RGBA[2];
	float a = color.RGBA[3];

	std::stringstream stream(text);
	std::string line;

	float y = 0.0f;
	int i = 0;
	auto DrawList = ImGui::GetForegroundDrawList();

	//for (const auto& ent : enemyHeroes) {
	//	int healthbarOffset = ent->Member<int>(0xc6c);
	//	auto entPos = ent->GetPos();
	//	float manaPercent = ent->GetMana() / ent->GetMaxMana();
	//	entPos.x += Config::OffsetX;
	//	entPos.y += Config::OffsetY;
	//	entPos.z += healthbarOffset + Config::OffsetZ;
	//	int x = 0, y = 0;
	//	Signatures::Scripts::WorldToScreen(entPos, &x, &y, nullptr);
	//	DrawList->AddRectFilledMultiColor(
	//		ImVec2(x - 25, y - 3), 
	//		ImVec2(x + 25 * manaPercent, y + 3),
	//		// yes, same thing 4 times over, such is ImGui :)
	//		ImGui::GetColorU32(ImVec4(0, 0, 1, 1)),
	//		ImGui::GetColorU32(ImVec4(0, 0, 1, 1)),
	//		ImGui::GetColorU32(ImVec4(0, 0, 1, 1)),
	//		ImGui::GetColorU32(ImVec4(0, 0, 1, 1)));
	//}
	while (std::getline(stream, line))
	{
		ImVec2 textSize = pFont->CalcTextSizeA(size, FLT_MAX, 0.0f, line.c_str());
		if (center)
		{
			DrawList->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) + 1, (pos.y + textSize.y * i) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, a / 255)), line.c_str());
			DrawList->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) - 1, (pos.y + textSize.y * i) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, a / 255)), line.c_str());
			DrawList->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) + 1, (pos.y + textSize.y * i) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, a / 255)), line.c_str());
			DrawList->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) - 1, (pos.y + textSize.y * i) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, a / 255)), line.c_str());
			DrawList->AddText(pFont, size, ImVec2(pos.x - textSize.x / 2.0f, pos.y + textSize.y * i), ImGui::GetColorU32(ImVec4(r / 255, g / 255, b / 255, a / 255)), line.c_str());
		}
		else
		{
			DrawList->AddText(pFont, size, ImVec2((pos.x) + 1, (pos.y + textSize.y * i) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, a / 255)), line.c_str());
			DrawList->AddText(pFont, size, ImVec2((pos.x) - 1, (pos.y + textSize.y * i) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, a / 255)), line.c_str());
			DrawList->AddText(pFont, size, ImVec2((pos.x) + 1, (pos.y + textSize.y * i) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, a / 255)), line.c_str());
			DrawList->AddText(pFont, size, ImVec2((pos.x) - 1, (pos.y + textSize.y * i) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, a / 255)), line.c_str());
			DrawList->AddText(pFont, size, ImVec2(pos.x, pos.y + textSize.y * i), ImGui::GetColorU32(ImVec4(r / 255, g / 255, b / 255, a / 255)), line.c_str());
		}

		y = pos.y + textSize.y * (i + 1);
		i++;
	}

	return y;
}

CDOTAParticleManager::ParticleWrapper particleWrap{};
uintptr_t WINAPI HackThread(HMODULE hModule) {
	// Initialize MinHook.
	if (MH_Initialize() != MH_OK)
	{
		return 1;
	}

	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);

	CurProcId = GetCurrentProcessId();
	CurProcHandle = OpenProcess(PROCESS_ALL_ACCESS, TRUE, CurProcId);

	std::cout << "works!" << std::endl;
	Interfaces::InitInterfaces();
	Interfaces::LogInterfaces();
	//Interfaces::CVar->DumpConVarsToFile("H:\\SchemaDump\\convars.txt");
	Interfaces::CVar->DumpConVarsToMap();

	Schema::SchemaDumpToMap("client.dll", "C_DOTA_BaseNPC_Hero");
	Schema::SchemaDumpToMap("client.dll", "C_DOTAPlayerController");
	Schema::SchemaDumpToMap("client.dll", "C_DOTA_UnitInventory");

	Signatures::InitSignatures();
	Signatures::LogSignatures();

	Globals::InitGlobals();
	Hooks::SetUpByteHooks();

	VMTs::Entity = std::unique_ptr<VMT>(new VMT(Interfaces::EntitySystem));
	VMTs::Entity->HookVM(Hooks::OnAddEntity, 14);
	VMTs::Entity->HookVM(Hooks::OnRemoveEntity, 15);
	VMTs::Entity->ApplyVMT();
	Hooks::InitVirtualHooks();

	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
		return 1;

	// GL 3.0 + GLSL 130
	const char* glsl_version = "#version 130";
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

	{
		
		auto objCache = Interfaces::GCClient->GetObjCache();
		auto objTypeCacheList = objCache->GetTypeCacheList();
		auto message = objTypeCacheList[1]->GetProtobufSO()->GetPObject();
		auto str = message->DebugString();


	}

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

	io.Fonts->AddFontFromFileTTF(R"(C:\Windows\Fonts\consola.ttf)", 16.0f, nullptr, io.Fonts->GetGlyphRangesDefault());
	auto font = io.Fonts->AddFontFromFileTTF(R"(C:\Windows\Fonts\trebuc.ttf)", 80.0f, nullptr, io.Fonts->GetGlyphRangesDefault());

	bool menuVisible = false;
	bool featuresMenuVisible = false;
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

		if (menuVisible) {
			ImGui::Begin("Main");
			if (ImGui::Button("Features"))
				featuresMenuVisible = !featuresMenuVisible;


			if (ImGui::Button("EXIT", ImVec2(0, 50)))
				glfwSetWindowShouldClose(window, 1);

			ImGui::End();

#ifdef _DEBUG
			ImGui::Begin("Debug functions", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

			if (ImGui::Button("Log Entities")) {
				LogEntities();
			}
			if (ImGui::Button("Log Inventory")) {
				auto selected = localPlayer->GetSelectedUnits();
				auto ent = (BaseNpc*)Interfaces::EntitySystem->GetEntity(selected[0]);
				LogInvAndAbilities(ent);
			}

			if (ImGui::Button("Log Modifiers")) {
				auto selected = localPlayer->GetSelectedUnits();
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
					className != nullptr
					)
					std::cout << className << ' ' << debugEntIdx
					<< " -> " << ent << '\n';
			}

			ImGui::End();
#endif // _DEBUG


			if (featuresMenuVisible) {
				ImGui::Begin("Features", &featuresMenuVisible, ImGuiWindowFlags_::ImGuiWindowFlags_AlwaysAutoResize);

				if (ImGui::Button("Circle drawing"))
					circleMenuVisible = !circleMenuVisible;

				// https://github.com/SK68-ph/Shadow-Dance-Menu
				ImGui::ListBox(
					"Change weather",
					&Config::WeatherListIdx,
					UIState::WeatherList,
					IM_ARRAYSIZE(UIState::WeatherList),
					3);

				ImGui::Checkbox("Auto-use Hand of Midas", &Config::AutoMidasEnabled);
				ImGui::Checkbox("Automatically pick up Bounty runes", &Config::AutoRunePickupEnabled);


				if (ImGui::CollapsingHeader("Visible by Enemy")) {
					ImGui::Checkbox("Show HIDDEN/DETECTED text", &Config::VBEShowText);
					ImGui::Checkbox("Show a circle under the hero when visible", &Config::VBEShowParticle);
				}
				if (ImGui::CollapsingHeader("Illusion coloring")) {
					ImGui::ColorEdit3("Color", &Config::IllusionColor.x);
				}
				if (ImGui::CollapsingHeader("AutoWand")) {
					ImGui::Checkbox("Auto-use Faerie Fire and Magic Stick", &Config::AutoWandEnabled);
					ImGui::SliderFloat("Faerie Fire HP Treshold", &Config::AutoHealFaerieFireHPTreshold, 0, 100, "%.1f");

					ImGui::Separator();

					ImGui::SliderFloat("Magic Stick/Wand/Holy Locket HP Treshold", &Config::AutoHealWandHPTreshold, 0, 100, "%.1f");
					ImGui::SliderInt("Minimum charges", &Config::AutoHealWandMinCharges, 1, 20);
				}
				ImGui::Checkbox("Auto-buy Tome of Knowledge", &Config::AutoBuyTome);
				ImGui::SliderFloat("Camera distance", &Config::CameraDistance, 1200, 3000, "%.1f");

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
							CDOTAParticleManager::ParticleAttachment_t::PATTACH_ABSORIGIN_FOLLOW,
							(BaseEntity*)assignedHero
						).particle
							->SetControlPoint(1, &color)
							->SetControlPoint(2, &radius)
							->SetControlPoint(3, &Vector3::Zero);
					}
					ImGui::End();
				}

			}
		}
		if (IsInMatch && Config::VBEShowText)
			DrawTextForeground(window, font, UIState::HeroVisibleToEnemy ? "DETECTED" : "HIDDEN", ImVec2(1920 / 2, 1080 * 3 / 4), 80.0f, Color(200, 200, 200, 255), true);

		//if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_Insert, false)) {
		if (IsKeyPressed(VK_INSERT)) {
			glfwSetWindowAttrib(window, GLFW_MOUSE_PASSTHROUGH, menuVisible);
			menuVisible = !menuVisible;
		}

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

	if (IsInMatch) {
		Modules::VBE.Reset();
		if (Globals::GameEventManager)
			for (auto& listener : CGameEventManager::EventListeners)
				Globals::GameEventManager->RemoveListener(listener.get());
		CGameEventManager::EventListeners.clear();
		CVarSystem::CVar.clear();
	}

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

