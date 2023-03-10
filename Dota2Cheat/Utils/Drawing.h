#pragma once
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h> // Will drag system OpenGL headers
#include "../stb_image.h"
#include <string>
#include <sstream>
#include "../SDK/Base/Color.h"

void DrawRect(const ImVec2& topLeft, const ImVec2& size, const ImVec4& color);
void HelpMarker(const char* desc);
//credits to SMBB from UnknownCheats
//https://www.unknowncheats.me/forum/direct3d/244074-imgui-d3d11-text-drawing.html
float DrawTextForeground(ImFont* pFont, const std::string& text, const ImVec2& pos, float size, Color color, bool center);

struct TextureData {
	const char* filename{};
	ImTextureID glTex{};
	int width{}, height{};
};

bool LoadTexture(const char* filename, TextureData& data);