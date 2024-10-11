#pragma once
#include <string>
#include <unordered_map>
#include <d3d11.h>

// Operates in VPK directory panorama/images
inline class CAssetManager {

	struct VTexDir {
		std::string dir;
		std::unordered_map<std::string, ID3D11ShaderResourceView*> cache;

		VTexDir(std::string_view dir) : dir(dir) {}

		// Loads in-place and in the render thread
		// Multithreadedness really does a number on your architecture

		ID3D11ShaderResourceView* Load(const std::string& file, std::string postfix = "png");
		void LoadDeferred(ID3D11ShaderResourceView** dest, const std::string& file, std::string postfix = "png");
	};
public:
	VTexDir
		heroIcons{ "panorama/images/heroes/icons" },
		spellIcons{ "panorama/images/spellicons" },
		items{ "panorama/images/items" };

	// For uncommon uses
	VTexDir Directory(const std::string& dir) const {
		return VTexDir(dir);
	}

	VTexDir operator[](const std::string& dir) const {
		return Directory(dir);
	}

} assets;