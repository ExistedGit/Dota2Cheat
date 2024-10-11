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
#include <queue>
#include "../Data/DrawData.h"

// Texture management system
// Caches loaded textures which you can get by the name identifier
inline class TextureManager {
	using tex_ptr = ID3D11ShaderResourceView*;

	struct DeferredInit {
		using callback_t = void(*)(DeferredInit&);

		unsigned width, height;
		unsigned char* data;
		tex_ptr* out;
		callback_t callback; // for stuff like clearing memory
	};
	std::queue<DeferredInit> loadingQueue;
public:
	void InitTexture(unsigned image_width,
		unsigned image_height,
		const unsigned char* image_data,
		tex_ptr* out_srv) const;

	void InitTextureDeferred(unsigned width, unsigned height, unsigned char* data, tex_ptr* out, DeferredInit::callback_t callback = nullptr) {
		loadingQueue.emplace(
			DeferredInit{
			.width = width,
			.height = height,
			.data = data,
			.out = out,
			.callback = callback
			}
		);
	};

	template<size_t size>
	bool LoadTextureFromMemory(unsigned char const (&data)[size], tex_ptr* tex) {
		return LoadTextureFromMemory(data, size, tex);
	}
	bool LoadTextureFromMemory(unsigned char* data, size_t size, tex_ptr* tex);

	bool LoadTextureFromFile(std::string_view filename, tex_ptr* tex);

	void ExecuteLoadCycle() {
		while (!loadingQueue.empty()) {
			auto& data = loadingQueue.front();

			InitTexture(data.width, data.height, data.data, data.out);
			if (data.callback) data.callback(data);

			loadingQueue.pop();
		}
	}

} texManager;