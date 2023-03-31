#pragma once
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h> // Will drag system OpenGL headers
#include "../stb_image.h"
#include <string>
#include <sstream>
#include "../SDK/Base/Color.h"
#include <unordered_map>

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