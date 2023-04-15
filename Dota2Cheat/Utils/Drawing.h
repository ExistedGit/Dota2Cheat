#pragma once
#include "../SDK/pch.h"
#include <imgui/imgui.h>
#include <imgui/imgui_stdlib.h>
#include <imgui/imgui_internal.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h> // Will drag system OpenGL headers
#include "../stb_image.h"
#include <string>
#include <sstream>
#include <unordered_map>

// Converts a 3D position in Dota's world to a minimap
inline ImVec2 ImVecFromVec2D(const Vector2D& vec) {
	return ImVec2{ vec.x,vec.y };
}

// Credit to Wolf49406
inline ImVec2 WorldToMap(const Vector& EntityPos) {
	if (!GameSystems::MinimapRenderer)
		return { 0,0 };
	auto MinimapSize = GameSystems::MinimapRenderer->GetMinimapSize();
	auto MinimapBounds = GameSystems::MinimapRenderer->MinimapBounds;

	// Actual Minimap is 94% from MinimapSize above (bc of borders I guess)
	auto ActualMinimapSize = static_cast<float>(MinimapSize.x * 0.94);
	auto MinimapPosMin = Vector2D(0, static_cast<float>(GameData.ScreenSize.y - ActualMinimapSize));

	if (Signatures::IsHUDFlipped()) {
		float offset = GameData.ScreenSize.x - ActualMinimapSize;
		MinimapPosMin.x = MinimapPosMin.x + offset;
	}

	Vector2D Scaler = MinimapBounds / ActualMinimapSize * 2;
	auto PosOnMinimap = MinimapPosMin + (MinimapBounds / Scaler) - (Vector2D{ EntityPos.x, EntityPos.y } / Scaler);

	return ImVecFromVec2D(PosOnMinimap);
}


void DrawRect(const ImVec2& topLeft, const ImVec2& size, const ImVec4& color, float thickness = 1.0f);
void DrawRectFilled(const ImVec2& topLeft, const ImVec2& size, const ImVec4& color);
void HelpMarker(const char* desc);
//credits to SMBB from UnknownCheats
//https://www.unknowncheats.me/forum/direct3d/244074-imgui-d3d11-text-drawing.html
float DrawTextForeground(ImFont* pFont, const std::string& text, const ImVec2& pos, float size, const ImVec4& color, bool center, bool outline = true);

struct TextureData {
	const char* filePath{};
	ImTextureID glTex{};
	int width{}, height{};
};


// Texture management system
// Caches loaded textures which you can get by the name identifier
class TextureManager {
	std::unordered_map<std::string, TextureData> namedTex;
	std::map<std::string, TextureData*> loadingQueue;
	bool requiresUnload = false;
public:
	TextureData* GetNamedTexture(const std::string& name) {
		if (!namedTex.count(name))
			return nullptr;
		return &namedTex[name];
	}

	bool LoadTexture(const char* filename, TextureData& data);
	bool LoadTextureNamed(const char* filename, TextureData& data, const std::string& texName) {
		auto result = LoadTexture(filename, data);
		namedTex[texName] = data;
		return result;
	};

	void QueueTextureUnload()
	{
		requiresUnload = true;
	}

	void ExecuteUnloadCycle() {
		if (!requiresUnload)
			return;

		for (auto& [_, data] : namedTex)
			glDeleteTextures(1, (const GLuint*)&data.glTex);
		namedTex.clear();
	}

	void QueueForLoading(const std::string& filename, const std::string& texName) {
		if (!namedTex.count(texName))
			loadingQueue[filename] = &namedTex[texName];
	}

	void ExecuteLoadCycle() {
		for (auto& [path, data] : loadingQueue)
			LoadTexture(path.c_str(), *data);

		loadingQueue.clear();
	}

};
inline TextureManager texManager;