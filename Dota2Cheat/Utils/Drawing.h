#pragma once
#include "../pch.h"
#include <imgui/imgui.h>
#include <imgui/imgui_stdlib.h>
#include <imgui/imgui_internal.h>
#include <imgui/imgui_impl_win32.h>
#include <imgui/imgui_impl_dx11.h>

void DrawRect(const ImVec2& topLeft, const ImVec2& size, const ImVec4& color, float thickness = 1.0f);
void DrawRectFilled(const ImVec2& topLeft, const ImVec2& size, const ImVec4& color);
void HelpMarker(const char* desc);
//credits to SMBB from UnknownCheats
//https://www.unknowncheats.me/forum/direct3d/244074-imgui-d3d11-text-drawing.html
float DrawTextForeground(ImFont* pFont, const std::string& text, const ImVec2& pos, float size, const ImVec4& color, bool center, bool outline = true);