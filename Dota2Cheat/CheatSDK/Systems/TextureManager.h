#pragma once
#include <string>
#include <map>
#include <sstream>
#include <unordered_map>

#include <imgui/imgui.h>
#include <imgui/imgui_stdlib.h>
#include <imgui/imgui_internal.h>
#include <imgui/imgui_impl_win32.h>
#include <imgui/imgui_impl_dx11.h>

#include <d3d11.h>
#include <dxgi.h>
#include "../Data/DrawData.h"

// Texture management system
// Caches loaded textures which you can get by the name identifier
inline class TextureManager {
	std::unordered_map<std::string, ID3D11ShaderResourceView*> namedTex;
	std::map<std::string, ID3D11ShaderResourceView**> loadingQueue;
	bool requiresUnload = false;
	void InitDX11Texture(int image_width,
		int image_height,
		unsigned char* image_data,
		ID3D11ShaderResourceView** out_srv);
public:
	ID3D11ShaderResourceView* GetNamedTexture(const std::string& name) {
		return namedTex[name];
	}

	template<size_t size>
	bool LoadTextureFromMemory(unsigned char const (&data)[size], ID3D11ShaderResourceView** tex) {
		return LoadTextureFromMemory(data, size, tex);
	}
	bool LoadTextureFromMemory(unsigned char* data, size_t size, ID3D11ShaderResourceView** tex);

	bool LoadTextureFromFile(const char* filename, ID3D11ShaderResourceView** tex);
	bool LoadTextureNamed(const char* filename, ID3D11ShaderResourceView** tex, const std::string& texName);

	void QueueTextureUnload()
	{
		requiresUnload = true;
	}

	//void ExecuteUnloadCycle() {
	//	if (!requiresUnload)
	//		return;

	//	for (auto& [_, tex] : namedTex)
	//		glDeleteTextures(1, (const GLuint*)&tex);
	//	namedTex.clear();
	//}

	void QueueForLoading(const std::string& filename, const std::string& texName) {
		if (!namedTex.count(texName))
			loadingQueue[filename] = &namedTex[texName];
	}

	void ExecuteLoadCycle() {
		for (auto& [path, tex] : loadingQueue) {
			// LogF(LP_INFO, "Loading image: {}", path);
			LoadTextureFromFile(path.c_str(), tex);
		}

		loadingQueue.clear();
	}

} texManager;