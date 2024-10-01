#pragma once
#include <string>
#include <unordered_map>
#include <d3d11.h>

// Operates in VPK directory panorama/images
inline class CAssetManager {
	const static inline std::string prefix = "panorama/images/";

	struct VTexDir {
		std::string dir;
		std::unordered_map<std::string, ID3D11ShaderResourceView*> cache;

		VTexDir(std::string_view dir) : dir(dir) {}

		ID3D11ShaderResourceView* Load(const std::string& file, std::string postfix = "png");
	};
public:
	VTexDir
		heroIcons{ "heroes/icons" },
		spellIcons{ "spellicons" },
		items{ "items" };

	// For uncommon uses
	VTexDir Directory(const std::string& dir) const {
		return VTexDir(dir);
	}

	VTexDir operator[](const std::string& dir) const {
		return Directory(dir);
	}

} assets;