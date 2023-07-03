#include "Drawing.h"

void DrawRect(const ImVec2& topLeft, const ImVec2& size, const ImVec4& color, float thickness) {
	auto DrawList = ImGui::GetForegroundDrawList();
	DrawList->AddRect(
		topLeft, ImVec2(size.x + topLeft.x, size.y + topLeft.y),
		ImGui::GetColorU32(color),
		0,
		0,
		thickness
	);
}

void DrawRectFilled(const ImVec2& topLeft, const ImVec2& size, const ImVec4& color) {
	auto DrawList = ImGui::GetForegroundDrawList();
	DrawList->AddRectFilled(
		topLeft, ImVec2(size.x + topLeft.x, size.y + topLeft.y),
		ImGui::GetColorU32(color)
	);
}

//credits to SMBB from UnknownCheats
//https://www.unknowncheats.me/forum/direct3d/244074-imgui-d3d11-text-drawing.html
float DrawTextForeground(ImFont* pFont, const std::string& text, const ImVec2& pos, float size, const ImVec4& color, bool center, bool outline)
{
	auto black = ImVec4(0, 0, 0, color.w);

	std::stringstream stream(text);
	std::string line;

	float y = 0.0f;
	int i = 0;
	auto DrawList = ImGui::GetForegroundDrawList();

	while (std::getline(stream, line))
	{
		ImVec2 textSize = pFont->CalcTextSizeA(size, FLT_MAX, 0.0f, line.c_str());
		if (center)
		{
			if (outline) {
				DrawList->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) + 1, (pos.y + textSize.y * i) + 1), ImGui::GetColorU32(black), line.c_str());
				DrawList->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) - 1, (pos.y + textSize.y * i) - 1), ImGui::GetColorU32(black), line.c_str());
				DrawList->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) + 1, (pos.y + textSize.y * i) - 1), ImGui::GetColorU32(black), line.c_str());
				DrawList->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) - 1, (pos.y + textSize.y * i) + 1), ImGui::GetColorU32(black), line.c_str());
			}
			DrawList->AddText(pFont, size, ImVec2(pos.x - textSize.x / 2.0f, pos.y + textSize.y * i), ImGui::GetColorU32(color), line.c_str());
		}
		else
		{
			if (outline) {

				DrawList->AddText(pFont, size, ImVec2((pos.x) + 1, (pos.y + textSize.y * i) + 1), ImGui::GetColorU32(black), line.c_str());
				DrawList->AddText(pFont, size, ImVec2((pos.x) - 1, (pos.y + textSize.y * i) - 1), ImGui::GetColorU32(black), line.c_str());
				DrawList->AddText(pFont, size, ImVec2((pos.x) + 1, (pos.y + textSize.y * i) - 1), ImGui::GetColorU32(black), line.c_str());
				DrawList->AddText(pFont, size, ImVec2((pos.x) - 1, (pos.y + textSize.y * i) + 1), ImGui::GetColorU32(black), line.c_str());
			}
			DrawList->AddText(pFont, size, ImVec2(pos.x, pos.y + textSize.y * i), ImGui::GetColorU32(color), line.c_str());
		}

		y = pos.y + textSize.y * (i + 1);
		i++;
	}

	return y;
}

void HelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

