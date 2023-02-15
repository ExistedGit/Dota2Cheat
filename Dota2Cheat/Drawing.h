#pragma once
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <string>
#include <sstream>
#include "SDK/color.h"

//credits to SMBB from UnknownCheats
//https://www.unknowncheats.me/forum/direct3d/244074-imgui-d3d11-text-drawing.html
inline float DrawTextForeground(GLFWwindow* wnd, ImFont* pFont, const std::string& text, const ImVec2& pos, float size, Color color, bool center)
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
