#include "LoadingMenu.h"
#include "../Utils/Drawing.h"
#include "../CheatSDK/include.h"

void Menus::LoadingMenu::Draw() {
	auto drawList = ImGui::GetForegroundDrawList();
	auto screenSize = Panorama::DotaHud->GetScreenSize();

	// Dark overlay
	drawList->AddRectFilled({ 0,0 }, { screenSize.x , screenSize.y }, ImColor{ 1., 1. , 1. , 0.6f });

	// Progress bar
	const float barThickness = 6;
	drawList->AddRectFilled({ 0, screenSize.y - barThickness }, { screenSize.x * ((float)progress / progressMax), screenSize.y }, ImColor{ 255,255,255 });
}

void Menus::LoadingMenu::DrawLaunchDialogue() {
	auto fontSize = 30.f;
	static auto x = 0.f;
	static int sign = 1;
	static int waitFrames = 0;

	if (!waitFrames)
		x += 0.01 * sign;

	if (x > 1 || x < -1) {
		//if (x < 0)
		waitFrames = 30;

		sign = -sign;
		x += 0.01 * sign;
	}

	if (waitFrames > 0)
		waitFrames--;



	auto drawList = ImGui::GetForegroundDrawList();
	auto screenSize = ImVec2{ 1920,1080 };

	// Dark overlay
	drawList->AddRectFilled({ 0,0 }, screenSize, ImColor{ 0., 0., 0., 0.6f });
	auto text = "Dota2Cheat loaded, press INSERT";
	//drawList->AddText(screenSize / 2 - ImVec2{ ImGui::CalcTextSize(text).x / 2, 0 }, ImColor{ 255,255,255 }, text);
	DrawTextForeground(DrawData.GetFont("Monofonto", 30), text, screenSize / 2, 30 + 6 * ((-pow(x, 2) + 1) / 2), ImColor{ 255,255,255 }, true);
}
