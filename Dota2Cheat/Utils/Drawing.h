#pragma once
#include "../SDK/pch.h"
#include <imgui/imgui.h>
#include <imgui/imgui_stdlib.h>
#include <imgui/imgui_internal.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h> // Will drag system OpenGL headers
#include <stb_image.h>
#include <string>
#include <sstream>
#include <unordered_map>

// Converts a 3D position in Dota's world to a minimap
inline ImVec2 ImVecFromVec2D(const Vector2D& vec) {
	return ImVec2{ vec.x,vec.y };
}

// Credit to Wolf49406
ImVec2 WorldToMap(const Vector& EntityPos);


void DrawRect(const ImVec2& topLeft, const ImVec2& size, const ImVec4& color, float thickness = 1.0f);
void DrawRectFilled(const ImVec2& topLeft, const ImVec2& size, const ImVec4& color);
void HelpMarker(const char* desc);
//credits to SMBB from UnknownCheats
//https://www.unknowncheats.me/forum/direct3d/244074-imgui-d3d11-text-drawing.html
float DrawTextForeground(ImFont* pFont, const std::string& text, const ImVec2& pos, float size, const ImVec4& color, bool center, bool outline = true);

// Texture management system
// Caches loaded textures which you can get by the name identifier
class TextureManager {
	std::unordered_map<std::string, ImTextureID> namedTex;
	std::map<std::string, ImTextureID*> loadingQueue;
	bool requiresUnload = false;
public:
	ImTextureID GetNamedTexture(const std::string& name) {
		return namedTex[name];
	}

	bool LoadTexture(const char* filename, ImTextureID& tex);
	bool LoadTextureNamed(const char* filename, ImTextureID& tex, const std::string& texName) {
		auto result = LoadTexture(filename, tex);
		namedTex[texName] = tex;
		return result;
	}

	void QueueTextureUnload()
	{
		requiresUnload = true;
	}

	void ExecuteUnloadCycle() {
		if (!requiresUnload)
			return;

		for (auto& [_, tex] : namedTex)
			glDeleteTextures(1, (const GLuint*)&tex);
		namedTex.clear();
	}

	void QueueForLoading(const std::string& filename, const std::string& texName) {
		if (!namedTex.count(texName))
			loadingQueue[filename] = &namedTex[texName];
	}

	void ExecuteLoadCycle() {
		for (auto& [path, tex] : loadingQueue) {
			// LogF(LP_INFO, "Loading image: {}", path);
			LoadTexture(path.c_str(), *tex);
		}

		loadingQueue.clear();
	}

};
inline TextureManager texManager;