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
	using tex_ptr = ID3D11ShaderResourceView*;

	std::unordered_map<std::string, tex_ptr> namedTex;
	std::map<std::string, tex_ptr*> loadingQueue;
	bool requiresUnload = false;
public:
	tex_ptr GetNamedTexture(const std::string& name) {
		return namedTex[name];
	}

	void InitDX11Texture(unsigned image_width,
		unsigned image_height,
		unsigned char* image_data,
		tex_ptr* out_srv) const;

	template<size_t size>
	bool LoadTextureFromMemory(unsigned char const (&data)[size], tex_ptr* tex) {
		return LoadTextureFromMemory(data, size, tex);
	}
	bool LoadTextureFromMemory(unsigned char* data, size_t size, tex_ptr* tex);

	bool LoadTextureFromFile(std::string_view filename, tex_ptr* tex);
	bool LoadTextureNamed(std::string_view filename, tex_ptr* tex, const std::string& texName) {
		auto result = LoadTextureFromFile(filename, tex);
		namedTex[texName] = *tex;
		return result;
	}

	void QueueForLoading(const std::string& filename, const std::string& texName) {
		if (!namedTex.count(texName))
			loadingQueue[filename] = &namedTex[texName];
	}

	void ExecuteLoadCycle() {
		for (auto& [path, tex] : loadingQueue) {
			LoadTextureFromFile(path, tex);
		}

		loadingQueue.clear();
	}

} texManager;